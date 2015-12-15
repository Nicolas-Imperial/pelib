/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

extern "C"{
#include <igraph.h>
}
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cmath>

#include <pelib/Schedule.hpp>
#include <pelib/SVGSchedule.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/Task.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>

#ifdef debug
#undef debug
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

using namespace pelib;
using namespace std;
using namespace Cairo;

SVGSchedule::SVGSchedule()
{
	this->ratio = 4.0 / 3;
	this->showCores = false;
	this->showFrequencies = true;
	this->linearFrequencyRadiant = true;
	this->lowFrequencyRGBA = -1;
	this->highFrequencyRGBA = 255;
}

SVGSchedule::SVGSchedule(double deadline, float ratio, bool showCores, bool showFrequencies, bool linearFrequencyRadiant, unsigned int lowFrequencyRGBA, unsigned int higFrequencyRGBA)
{
	this->ratio = ratio;
	this->showCores = showCores;
	this->showFrequencies = showFrequencies;
	this->linearFrequencyRadiant = linearFrequencyRadiant;
	this->lowFrequencyRGBA = lowFrequencyRGBA;
	this->highFrequencyRGBA = highFrequencyRGBA;
}

SVGSchedule::~SVGSchedule()
{
	// Do nothing
}

void
SVGSchedule::dump(ostream& os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const
{
	dump(os, &data, &tg, &pt);
}

// sigc callback class initialized with an output stream
class Slot: public sigc::mem_functor2<ErrorStatus, Slot, const unsigned char*, unsigned int>
{
	public:
		Slot(ostream &out): sigc::mem_functor2<ErrorStatus, Slot, const unsigned char* , unsigned int>(sigc::mem_functor2<ErrorStatus, Slot, const unsigned char*, unsigned int>(&Slot::write))
		{
			this->out = &out;
		}
		
		ErrorStatus
		write(const unsigned char* data, unsigned int length)
		{
			bool success = true;
			for(size_t i = 0; i < length && success; i++)
			{
				success = success && (*this->out << data[i]).good();
			}

			return success ? CAIRO_STATUS_SUCCESS: CAIRO_STATUS_WRITE_ERROR;
		};
	
		ErrorStatus
		operator()(const unsigned char* data, unsigned int length)
		{
			return write(data, length);
		}

	private:
		ostream *out;
};

class Canvas
{
	public:
		Canvas(ostream &out, double height, double width, double thickness, double magnify, size_t p, double* F, size_t f)
		{
#ifdef CAIRO_HAS_SVG_SURFACE
			// Compute canvas size, taking thickness and magnification into account
			this->thickness = thickness;
			this->magnify = magnify;
			this->magnify = height >= magnify ? magnify : magnify / height;
			height = height * this->magnify;
			this->absthick = thickness * height;
			this->height = height + absthick;
			this->width = width * this->magnify + absthick;
			this->legend_size = (((this->height - this->absthick) / (f + 2)) + this->absthick) * 3; // Space for legend
			this->margin = 0.02 * (this->width - absthick - this->legend_size); // space between extreme-left and extreme-right tasks and extremities of barriers
			this->core_size = (this->width - absthick - this->margin * 2 - this->legend_size) / p;
			this->fontSize = this->height > this->width ? this->height : this->width;

			// Create cairo surface into svg output stream
			Slot slot(out);
			debug(this->width);
			debug(this->height);
			debug(this->core_size);
			debug(this->absthick);
			debug(this->legend_size);
			debug(this->margin);
			this->surface = SvgSurface::create_for_stream(slot, this->width, this->height);
			this->cr = Cairo::Context::create(surface);

			// Set the pen thickness and style
			cr->set_line_width(this->absthick);
			cr->set_line_cap(LINE_CAP_ROUND);
			cr->set_line_join(LINE_JOIN_ROUND);
			setColor(255);
#else
#warning You must compile cairo with SVG support for this example to work.
#endif
		}

		void
		drawTask(double time, double width, double frequency, double start, double firstCore, string label)
		{
			// Task rectangle
			cr->rectangle((firstCore - 1) * this->core_size + absthick / 2 + margin, height - absthick / 2 - (start + time) * magnify, width * this->core_size, time * magnify);
			// Fill rectangle in solid red
			setColor(encode(0, 255, 3) + 255); // solid red
			cr->fill_preserve();
			// Trace stroke in solid back
			setColor(255);
			cr->stroke();

			// Add task name
			cairo_text_extents_t te;
			cairo_font_extents_t fe;
			cr->set_font_size(this->idealFontSize(time, width, label));
			cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
			cr->get_font_extents(fe);
			cr->get_text_extents(label, te);
			cr->move_to(absthick / 2 + margin + (firstCore - 1) * this->core_size + this->core_size * width / 2 - te.width / 2 - te.x_bearing, height - absthick / 2 - time * magnify / 2 - fe.descent + fe.height / 2);
			setColor(255);
			cr->show_text(label);
		}

		double
		idealFontSize(double time, double width, string label)
		{
			double font_size = this->fontSize;
			//double font_size = time * magnify > width * this->core_size ? time * magnify : magnify * width;
			double delta = font_size;
			int last = 0;
			double minError = 0.01 * this->width;
			double maxError = 0.03 * this->width;

			while(true)
			{
				cairo_text_extents_t te;
				cairo_font_extents_t fe;
				cr->set_font_size(font_size);
				cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
				cr->get_font_extents(fe);
				cr->get_text_extents(label, te);
				double text_width = te.width - te.x_bearing;
				double text_height = te.height;

				debug("##########################");
				debug(delta);
				debug(font_size);
				debug(text_width);
				debug(text_height);
				debug(height);
				debug(width);
				debug(width * this->core_size - 2 * absthick - minError);
				debug(time * this->magnify - 2 * absthick - minError);
				debug(width * this->core_size - 2 * absthick - maxError);
				debug(time * this->magnify - 2 * absthick - maxError);

				if((text_width >= width * this->core_size - 2 * absthick - minError) || (text_height >= time * this->magnify - 2 * absthick - minError))
				{
					delta = delta / 2;
					font_size = font_size - delta;
				}
				else if((text_width <= width * this->core_size - 2 * absthick - maxError) && (text_height <= time * this->magnify - 2 * absthick - maxError))
				{		
					font_size = font_size + delta;
				}
				else
				{
					return font_size;
				}
			}
		}

		void
		drawBarrier(double time)
		{
			cr->move_to(this->absthick / 2, height - time * magnify - this->absthick / 2); 
			cr->line_to(width - this->absthick / 2 - legend_size, height - time * magnify - this->absthick / 2);
			setColor(255); // solid black
			cr->stroke();
		}

	protected:
		unsigned int
		shiftLeft(unsigned int value, unsigned int shift)
		{
			return value << shift;
		}

		unsigned int
		shiftRight(unsigned int value, unsigned int shift)
		{
			return value >> shift;
		}

		unsigned char
		decode(unsigned int value, unsigned int position)
		{
			return shiftRight(value & shiftLeft(255, position * 8), position * 8);
		}

		unsigned int
		encode(unsigned int base, unsigned char value, unsigned int position)
		{
			return base - shiftLeft(decode(base, position), position * 8) + shiftLeft(value, position * 8);
		}

		void
		setColor(unsigned int rgba)
		{
			cr->set_source_rgba(decode(rgba, 3) / 255, decode(rgba, 2) / 255, decode(rgba, 1) / 255, decode(rgba, 0) / 255);
		}

		float thickness;
		double magnify;
		double height;
		double width;
		double absthick;
		double core_size;
		double margin;
		double legend_size;
		double fontSize;
		Cairo::RefPtr<Cairo::Context> cr;
		Cairo::RefPtr<Cairo::SvgSurface> surface;
};

void
SVGSchedule::dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt) const
{
	Schedule::table schedule = sched->getSchedule();
	set<Task> &tasks = (set<Task>&)tg->getTasks();
	set<string> task_ids;
	
	double max_stop_time = 0;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.first->getName());
			double stop_time = j->second.first->getStartTime() + j->second.first->runtime(j->second.first->getWidth(), j->second.first->getFrequency());
			if(stop_time > max_stop_time)
			{
				max_stop_time = stop_time;
			}
		}
	}

	double deadline = tg->getDeadline(pt);
	bool drawDeadline = deadline > 0;
	float thickness = 0.02;
	double magnify = 100;
	deadline = deadline > 0 ? deadline : max_stop_time;

	Canvas canvas(os, deadline, deadline * this->ratio, thickness, magnify, 4, NULL, 3);
	canvas.drawTask(deadline, 2, 0, 0, 3, "Hello");
	//canvas.drawTask(*sched->getTasks().begin(), 0, 3);
	canvas.drawBarrier(0);
	if(drawDeadline)
	{
		canvas.drawBarrier(deadline);
	}
#if 0
	// Instanciate a callback class for with output stream
	Slot slot(os);
	// Create a SVG surface written to os output stream via slot instance of cairo-specialized callback class
	Cairo::RefPtr<Cairo::SvgSurface> surface = SvgSurface::create_for_stream(slot, width, height);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

	// Set the pen thickness and style
	cr->set_line_width(absthick);
	cr->set_line_cap(LINE_CAP_ROUND);
	cr->set_line_join(LINE_JOIN_ROUND);
	
	// Draw deadline lines
	if(drawDeadline)
	{
		cr->save(); // save the state of the context
		cr->move_to(absthick / 2, absthick / 2); 
		cr->line_to(absthick / 2, height - absthick / 2);
		cr->stroke();
		cr->restore(); // color is back to black now
	}

	cr->save(); // save the state of the context
	cr->set_source_rgb(0.86, 0.85, 0.47);
	cr->paint(); // fill image with the color
	cr->restore(); // color is back to black now

	cr->save();
	// draw a border around the image
	cr->set_line_width(absthick); // make the line wider
	//cr->rectangle(0.0, 0.0, cairo_image_surface_get_width(surface->cobj()), height);
	cr->rectangle(absthick / 2, absthick / 2, width - absthick, height - absthick);
	cr->stroke();

	cr->set_source_rgba(0.0, 0.0, 0.0, 0.7);
	// draw a circle in the center of the image
	cr->arc(width / 2.0, height / 2.0, height / 4.0, 0.0, 2.0 * M_PI);
	cr->stroke();

	// draw a diagonal line
	cr->move_to(width / 4.0, height / 4.0);
	cr->line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
	cr->stroke();
	cr->restore();
	
	cr->show_page();
#endif
#if 0
	Schedule::table schedule = sched->getSchedule();
	set<Task> &tasks = (set<Task>&)tg->getTasks();
	set<string> task_ids;
	
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.first->getName());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
		<< "<schedule name=\"" << sched->getName() << "\" appname=\"" << sched->getAppName() << "\""
		//<< "makespan=\"" << target_makespan << "\" "
		<< ">" << endl;

	// Finds and set the precision required for this schedule
	std::streamsize old_precision = os.precision();
	float min_delta = FLT_MAX;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		float last_start = 0;
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task t = *j->second.first;
			if(t.getStartTime() > 0)
			{
				float delta = t.getStartTime() - last_start;
				if(delta < min_delta)
				{
					min_delta = delta;
				}
				/*
				os << "<!-- core : " << i->first << " -->" << endl;
				os << "<!-- task : " << t.getName() << " -->" << endl;
				os << "<!-- start_time = " << t.getStartTime() << " -->" << endl;
				os << "<!-- last_start = " << last_start << " -->" << endl;
				*/

				// update last time
				last_start = t.getStartTime();
			}
		}
	}
	std::streamsize precision = (std::streamsize)(ceil(-log10(min_delta)) + 1);
	//os << "<!-- precision = " << precision << " -->" << endl;

	os << setprecision(precision);
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p << "\">" << endl;
		Schedule::sequence core_schedule = i->second;
		int order = 0;
		double start = 0;

		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			string taskid = j->second.first->getName();
			Task t = *j->second.first;
			size_t task_index = std::distance(sched->getTasks().begin(), sched->getTasks().find(t));
			
			os << "  <task name=\"" << taskid << "\" "
				<< "start=\"" << (t.getStartTime() > 0 ? t.getStartTime() : start) << "\" "
				<< "frequency=\"" << t.getFrequency() << "\" "
				<< "width=\"" << t.getWidth() << "\" "
				<< "workload=\"" << t.getWorkload() << "\""
				<< "/>" << endl;

				set<Task>::iterator iter = tasks.begin();
				std::advance(iter, task_index);
				t.setMaxWidth(iter->getMaxWidth());
				t.setEfficiencyString(iter->getEfficiencyString());
				start += t.runtime(t.getWidth(), t.getFrequency());
		}
		os << " </core>" << endl;
	}
	os << setprecision(old_precision);

	os << "</schedule>" << endl;
	#endif
}

SVGSchedule*
SVGSchedule::clone() const
{
	return new SVGSchedule();
}
