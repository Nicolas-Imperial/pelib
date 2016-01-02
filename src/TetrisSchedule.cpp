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
#include <pelib/TetrisSchedule.hpp>

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

TetrisSchedule::TetrisSchedule()
{
	this->ratio = 4.0 / 3;
	this->showCores = false;
	this->showFrequencies = true;
	this->linearFrequencyRadiant = true;
	this->lowFrequencyRGBA = -1;
	this->highFrequencyRGBA = 255;
}

TetrisSchedule::TetrisSchedule(double deadline, float ratio, bool showCores, bool showFrequencies, bool linearFrequencyRadiant, unsigned int lowFrequencyRGBA, unsigned int higFrequencyRGBA)
{
	this->ratio = ratio;
	this->showCores = showCores;
	this->showFrequencies = showFrequencies;
	this->linearFrequencyRadiant = linearFrequencyRadiant;
	this->lowFrequencyRGBA = lowFrequencyRGBA;
	this->highFrequencyRGBA = highFrequencyRGBA;
}

TetrisSchedule::~TetrisSchedule()
{
	// Do nothing
}

void
TetrisSchedule::dump(ostream& os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const
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
		Canvas(ostream &out, double height, double width, double thickness, double magnify, size_t p, set<float> F, vector<unsigned int> colors)
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
			// Create cairo surface into svg output stream
			Slot slot(out);
#if 0
			debug(this->width);
			debug(this->height);
			debug(this->core_size);
			debug(this->absthick);
			debug(this->legend_size);
			debug(this->margin);
#endif
			this->surface = SvgSurface::create_for_stream(slot, this->width, this->height);
			this->cr = Cairo::Context::create(surface);
			this->colors = Canvas::makeGradient(colors, F);

			size_t f = F.size();
			this->legend_size = (((this->height - this->absthick) / (f + 2)) + this->absthick) * 3; // Space for legend
			cairo_text_extents_t te;
			cairo_font_extents_t fe;
			cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
			cr->get_text_extents("Time", te);
			cr->get_font_extents(fe);
			this->yaxis_size = 7 * absthick + te.height; // Space for y axis legend
			this->margin = 0.02 * (this->width - absthick - this->legend_size); // Space between extreme-left and extreme-right tasks and extremities of barriers
			this->core_size = (this->width - absthick - this->margin * 2 - this->legend_size - this->yaxis_size) / p;
			this->fontSize = 0;

			// Set the pen thickness and style
			cr->set_line_width(this->absthick);
			cr->set_line_cap(LINE_CAP_ROUND);
			cr->set_line_join(LINE_JOIN_ROUND);
			setColor(255); // solid black

			// Draw y axis
			// Vertical line
			cr->move_to((te.height + this->yaxis_size) / 2, height - this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size) / 2, this->absthick * 2.5); 
			cr->stroke();
			// Left arrow stroke
			cr->move_to((te.height + this->yaxis_size) / 2, this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size) / 2 - this->absthick * 2, this->absthick * 4.5); 
			cr->stroke();
			// Right arrow stroke
			cr->move_to((te.height + this->yaxis_size) / 2, this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size) / 2 + this->absthick * 2, this->absthick * 4.5); 
			cr->stroke();
			// Text
			cr->save();
			cr->move_to((te.height + this->yaxis_size) / 2 - this->absthick * 2.5, height / 2 + te.width / 2 - te.x_bearing); 
			cr->translate((te.height + this->yaxis_size) / 2 - this->absthick * 2.5, height / 2 + te.width / 2 - te.x_bearing); 
			cr->rotate_degrees(-90);
			cr->show_text("Time");
			cr->restore();
#else
#warning You must compile cairo with SVG support for this schedule output to work.
#endif
		}

		void
		drawTask(double time, double width, double frequency, double start, double firstCore, string label)
		{
#ifdef CAIRO_HAS_SVG_SURFACE
			// Task rectangle
			cr->rectangle((firstCore - 1) * this->core_size + absthick / 2 + margin + yaxis_size, height - absthick / 2 - (start + time) * magnify, width * this->core_size, time * magnify);
			// Fill rectangle in solid red
			unsigned int color = this->colors.find(frequency)->second;
			setColor(color);
			cr->fill_preserve();
			// Trace stroke in solid back
			setColor(255);
			cr->stroke();

			// Add task name
			if(this->fontSize > 0)
			{
				cr->set_font_size(this->fontSize);
				cairo_text_extents_t te;
				cairo_font_extents_t fe;
				cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
				cr->get_font_extents(fe);
				cr->get_text_extents(label, te);
				cr->move_to(absthick / 2 + margin + this->yaxis_size + (firstCore - 1) * this->core_size + this->core_size * width / 2 - te.width / 2 - te.x_bearing, height - absthick / 2 - start * magnify - time * magnify / 2 - fe.descent + fe.height / 2);
				setColor(255);
				cr->show_text(label);
			}
#endif
		}

		static
		map<float, unsigned int>
		makeGradient(vector<unsigned int> colors, set<float> values)
		{
			map<float, unsigned int> gradient;
			float minValue = *values.begin();
			float maxValue = *values.rbegin();

			for(set<float>::iterator i = values.begin(); i != values.end(); i++)
			{
				float value = *i;
				float position = (value - minValue) / (maxValue - minValue) * (colors.size() - 1);
				size_t minIndex = (size_t)floor(position);
				size_t maxIndex = (size_t)ceil(position);
				position = (position - minIndex);

				vector<unsigned int>::iterator minIter = colors.begin();
				std::advance(minIter, minIndex);
				unsigned int minColor = *minIter;
				unsigned char minRed = decode(minColor, 0);
				unsigned char minGreen = decode(minColor, 1);
				unsigned char minBlue = decode(minColor, 2);
				unsigned char minAlpha = decode(minColor, 3);

				vector<unsigned int>::iterator maxIter = colors.begin();
				std::advance(maxIter, maxIndex);
				unsigned int maxColor = *maxIter;
				unsigned char maxRed = decode(maxColor, 0);
				unsigned char maxGreen = decode(maxColor, 1);
				unsigned char maxBlue = decode(maxColor, 2);
				unsigned char maxAlpha = decode(maxColor, 3);

				unsigned char valueRed = minRed * (1 - position) + maxRed * position;
				unsigned char valueGreen = minGreen * (1 - position) + maxGreen * position;
				unsigned char valueBlue = minBlue * (1 - position) + maxBlue * position;
				unsigned char valueAlpha = minAlpha * (1 - position) + maxAlpha * position;
				unsigned int valueColor = valueRed + encode(0, valueGreen, 1) + encode(0, valueBlue, 2) + encode(0, valueAlpha, 3);

				gradient.insert(pair<float, int>(value, valueColor));
			}

			return gradient;
		}

		double
		idealFontSize(double time, double width, string label, double startSize)
		{
#ifdef CAIRO_HAS_SVG_SURFACE
			double font_size = startSize;
			double delta = font_size;
			double minError = 0 * this->absthick;
			double maxError = 0 * this->absthick;

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
#if 0
				debug("##########################");
				debug(delta);
				debug(font_size);
				debug(text_width);
				debug(text_height);
				debug(height);
				debug(width);
				debug(width * this->core_size);
				debug(time * this->magnify);
				debug(width * this->core_size - 2 * absthick - minError);
				debug(time * this->magnify - 2 * absthick - minError);
				debug(width * this->core_size - 2 * absthick - maxError);
				debug(time * this->magnify - 2 * absthick - maxError);
				debug(label);
#endif
				if(((text_width >= (width * this->core_size - 2 * absthick - minError)) || (text_height >= (time * this->magnify - 2 * absthick - minError))) && delta > 0)
				{
					delta = delta / 2;
					font_size = font_size - delta;
				}
				else if(((text_width <= (width * this->core_size - 2 * absthick - maxError)) && (text_height <= (time * this->magnify - 2 * absthick - maxError))) && delta > 0)
				{		
					font_size = font_size + delta;
				}
				else
				{
					return font_size;
				}
			}
#endif
		}

		void
		drawBarrier(double time)
		{
#ifdef CAIRO_HAS_SVG_SURFACE
			cr->move_to(this->absthick / 2 + this->yaxis_size, height - time * magnify - this->absthick / 2); 
			cr->line_to(width - this->absthick / 2 - legend_size, height - time * magnify - this->absthick / 2);
			setColor(255); // solid black
			cr->stroke();
#endif
		}

		double
		getFontSize() const
		{
			return this->fontSize;
		}

		void
		setFontSize(double size)
		{
			this->fontSize = size;
		}

		double
		getWidth() const
		{
			return this->width;
		}
		
		double
		getHeight() const
		{
			return this->height;
		}

		static unsigned int
		shiftLeft(unsigned int value, unsigned int shift)
		{
			return value << shift;
		}

		static unsigned int
		shiftRight(unsigned int value, unsigned int shift)
		{
			return value >> shift;
		}

		static unsigned char
		decode(unsigned int value, unsigned int position)
		{
			return shiftRight(value & shiftLeft(255, position * 8), position * 8);
		}

		static unsigned int
		encode(unsigned int base, unsigned char value, unsigned int position)
		{
			return base - shiftLeft(decode(base, position), position * 8) + shiftLeft(value, position * 8);
		}

	protected:
		void
		setColor(unsigned int rgba)
		{
			cr->set_source_rgba((float)decode(rgba, 3) / 255, (float)decode(rgba, 2) / 255, (float)decode(rgba, 1) / 255, (float)decode(rgba, 0) / 255);
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
		double yaxis_size;
		map<float, unsigned int> colors;
#ifdef CAIRO_HAS_SVG_SURFACE
		Cairo::RefPtr<Cairo::Context> cr;
		Cairo::RefPtr<Cairo::SvgSurface> surface;
#endif
};

void
TetrisSchedule::dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt) const
{
#ifdef CAIRO_HAS_SVG_SURFACE
	Schedule::table schedule = sched->getSchedule();
	set<string> task_ids;
	set<float> frequencies;

	for(set<const Core*>::iterator i = pt->getCores().begin(); i != pt->getCores().end(); i++)
	{
		const Core *core = *i;
		for(set<float>::iterator j = core->getFrequencies().begin(); j != core->getFrequencies().end(); j++)
		{
			frequencies.insert(*j);
		}
	}

	// Browse all tasks to find the time the last task stops	
	double max_stop_time = 0;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task task = *j->second.first;
			task.setWorkload(tg->getTasks().find(task)->getWorkload());
			task_ids.insert(task.getName());
			double stop_time = task.getStartTime() + task.runtime(task.getWidth(), task.getFrequency());
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

	vector<unsigned int> colors;
	unsigned int darkRed = Canvas::encode(0, 84, 3) + Canvas::encode(0, 0, 2) + Canvas::encode(0, 0, 1) + 255;
	unsigned int lightYellow = Canvas::encode(0, 255, 3) + Canvas::encode(0, 250, 2) + Canvas::encode(0, 165, 1) + 255;
	colors.push_back(lightYellow);
	colors.push_back(darkRed);
	Canvas canvas(os, deadline, deadline * this->ratio, thickness, magnify, pt->getCores().size(), frequencies, colors);

	// Browse all tasks to determine the biggest font size
	double startSize = canvas.getHeight() > canvas.getWidth() ? canvas.getHeight() : canvas.getWidth();
	double fontSize = startSize;
	for(set<Task>::const_iterator i = sched->getTasks().begin(); i != sched->getTasks().end(); i++)
	{
		Task task = *i;
		task.setWorkload(tg->getTasks().find(task)->getWorkload());
		double runtime = task.runtime(i->getWidth(), i->getFrequency());
		double width = i->getWidth();
		string name = i->getName();
		double idealSize = canvas.idealFontSize(runtime, width, name, startSize);
		if(idealSize <= fontSize)
		{
			fontSize = idealSize;
		}
	}
	canvas.setFontSize(fontSize);

	// Browse all tasks in schedule to draw them
	set<Task> drawn;
	for(Schedule::table::iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task task = *j->second.first;
			if(drawn.find(task) == drawn.end())
			{
				task.setWorkload(tg->getTasks().find(task)->getWorkload());
				double runtime = task.runtime(task.getWidth(), task.getFrequency());
				double width = task.getWidth();
				double start = task.getStartTime();
				size_t core = std::distance(schedule.begin(), i) + 1;
				string name = task.getName();
				canvas.drawTask(runtime, width, task.getFrequency(), start, core, name);
				drawn.insert(task);
			}
		}
	}

	canvas.drawBarrier(0);
	if(drawDeadline)
	{
		canvas.drawBarrier(deadline);
	}
#endif
}

TetrisSchedule*
TetrisSchedule::clone() const
{
	return new TetrisSchedule();
}

