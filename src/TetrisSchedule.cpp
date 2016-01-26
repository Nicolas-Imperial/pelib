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

float
TetrisSchedule::defaultRatio()
{
	return 4.0 / 3;
}

float
TetrisSchedule::defaultStrokeSize()
{
	return 1;
}

bool
TetrisSchedule::defaultFrequencyLegend()
{
	return true;
}

vector<unsigned int>
TetrisSchedule::defaultFrequencyColors()
{
	vector<unsigned int> colors;
	unsigned int darkRed = encode(0, 84, 3) + encode(0, 0, 2) + encode(0, 0, 1) + 255;
	unsigned int lightYellow = encode(0, 255, 3) + encode(0, 250, 2) + encode(0, 165, 1) + 255;
	colors.push_back(lightYellow);
	colors.push_back(darkRed);

	return colors;
}

bool
TetrisSchedule::defaultTaskLabel()
{
	return false;
}

TetrisSchedule::TetrisSchedule()
{
	this->ratio = defaultRatio();
	this->showFrequencies = defaultFrequencyLegend();
	this->useTaskName = defaultTaskLabel();
	this->colors = defaultFrequencyColors();
	this->strokeSize = defaultStrokeSize();
}

TetrisSchedule::TetrisSchedule(float ratio, bool showFrequencies, bool useTaskName, vector<unsigned int> frequency_colors, float strokeSize)
{
	this->ratio = ratio;
	this->showFrequencies = showFrequencies;
	this->colors = frequency_colors;
	this->useTaskName = useTaskName;
	this->strokeSize = strokeSize;
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
		Canvas(double height, double width, double thickness, double magnify, map<float, unsigned int> colors, bool drawDeadline, bool showFrequencies)
		{
			this->height = height;
			this->width = width;
			this->thickness = thickness;
			this->magnify = magnify;
			this->colors = colors;
			this->drawDeadline = drawDeadline;
			this->showFrequencies = showFrequencies;
		}
		
		void
		dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt, bool useTaskName, float strokeSize)
		{
#ifdef CAIRO_HAS_SVG_SURFACE
			// Compute canvas size, taking thickness and magnification into account
			this->magnify = height >= magnify ? magnify : magnify / height;
			height = height * this->magnify;
			this->absthick = thickness * height * strokeSize;
			this->height = height + absthick;
			this->width = width * this->magnify + absthick;

			// Create cairo surface into svg output stream
			Slot slot(os);
			this->surface = SvgSurface::create_for_stream(slot, this->width, this->height);
			this->cr = Cairo::Context::create(surface);

			// Partition drawing surface
			// Space for frequency legend
			if(this->showFrequencies)
			{
				size_t f = this->colors.size();
				this->legend_size = (((this->height - this->absthick) / (f + 2))); // Space for legend
			}
			else
			{
				this->legend_size = 0;
			}
			// Space for y axis legend
			this->setFontSize(4 * this->absthick);
			cairo_text_extents_t te;
			cairo_font_extents_t fe;
			cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
			cr->get_text_extents("Time", te);
			cr->get_font_extents(fe);
			this->yaxis_size = 4 * absthick - te.y_bearing;

			// Space between extreme-left and extreme-right tasks and extremities of barriers
			this->margin = 3 * absthick;
			size_t p = pt->getCores().size();
			this->core_size = (this->width - absthick - this->margin * 2 - this->legend_size - this->yaxis_size) / p;

			// Browse all tasks to determine the biggest font size that fits all tasks to draw
			double startSize = this->getHeight() > this->getWidth() ? this->getHeight() : this->getWidth();
			double fontSize = startSize;
			for(set<Task>::iterator i = sched->getTasks().begin(); i != sched->getTasks().end(); i++)
			{
				Task task = *i;
				task.setWorkload(tg->getTasks().find(task)->getWorkload());
				double runtime = task.runtime(i->getWidth(), i->getFrequency());
				double width = i->getWidth();
				string name;
				if(useTaskName)
				{
					name = i->getName();
				}
				else
				{
					stringstream ss;
					ss << std::distance(sched->getTasks().begin(), i) + 1;
					name = ss.str();
				}
				double idealSize = this->idealFontSize(runtime, width, name, startSize);
				if(idealSize <= fontSize)
				{
					fontSize = idealSize;
				}
			}

			// Check out font size of frequency legend
			float legend_height = 0;
			float legend_font_size = 0;
			map<float, string> freq_labels;
			if(this->showFrequencies)
			{
				legend_height = this->legend_size;
				float legend_width = legend_height / this->core_size;
				legend_font_size = legend_height;
				for(map<float, unsigned int>::const_iterator i = this->colors.begin(); i != this->colors.end(); i++)
				{
					float freq = i->first * (*pt->getCores().begin())->getFrequencyUnit();
					unsigned int letter_number = ((unsigned int)log10(freq)) / 3;
					float short_freq = freq;
					unsigned char letter;
					switch(letter_number)
					{
						case 0:
							letter = '\0';
						break;
						case 1:
							letter = 'K';
							short_freq /= 1000;
						break;
						case 2:
							letter = 'M';
							short_freq /= 1000;
							short_freq /= 1000;
						break;
						case 3:
							letter = 'G';
							short_freq /= 1000;
							short_freq /= 1000;
							short_freq /= 1000;
						break;
						break;
						case 4:
							letter = 'T';
							short_freq /= 1000;
							short_freq /= 1000;
							short_freq /= 1000;
							short_freq /= 1000;
						break;
						default:
							letter = 'X';
						break;
					}
					stringstream freq_label;
					if(letter != '\0')
					{
						freq_label << short_freq << letter;
					}
					else
					{
						freq_label << short_freq;
					}
					freq_labels.insert(pair<float, string>(freq, freq_label.str()));
					double ideal_legend_size = this->idealFontSize(legend_height, legend_width, freq_label.str(), (double)legend_height);
					if(ideal_legend_size < legend_font_size)
					{
						legend_font_size = ideal_legend_size;
					}	
				}
			}

			// Set the pen thickness and style
			cr->set_line_width(this->absthick);
			cr->set_line_cap(LINE_CAP_ROUND);
			cr->set_line_join(LINE_JOIN_ROUND);
			setColor(255); // solid black

			// Draw y axis legend
			// Vertical line
			cr->move_to((te.height + this->yaxis_size + absthick) / 2, height - this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size + absthick) / 2, this->absthick * 2.5); 
			cr->stroke();
			// Left arrow stroke
			cr->move_to((te.height + this->yaxis_size + absthick) / 2, this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size + absthick) / 2 - this->absthick * 2, this->absthick * 6.5); 
			cr->stroke();
			// Right arrow stroke
			cr->move_to((te.height + this->yaxis_size + absthick) / 2, this->absthick * 2.5); 
			cr->line_to((te.height + this->yaxis_size + absthick) / 2 + this->absthick * 2, this->absthick * 6.5); 
			cr->stroke();
			// Text
			cr->save();
			this->setFontSize(4 * this->absthick);
			cr->move_to(-te.y_bearing, height / 2 + te.width / 2 - te.x_bearing); 
			cr->translate(-te.y_bearing, height / 2 + te.width / 2 - te.x_bearing); 
			cr->rotate_degrees(-90);
			cr->show_text("Time");
			cr->restore();

			// Draw frequency legend
			if(this->showFrequencies)
			{
				for(map<float, unsigned int>::iterator i = this->colors.begin(); i != this->colors.end(); i++)
				{
					unsigned int color = i->second;
					size_t index = std::distance(this->colors.begin(), i);

					cr->rectangle(this->width - legend_height - absthick / 2, legend_height * (index + 1) + absthick / 2, legend_height, legend_height);
					setColor(color);
					cr->fill_preserve();
					setColor(255);
					cr->stroke();

					// Write Frequency
					float freq = i->first * (*pt->getCores().begin())->getFrequencyUnit();
					cr->set_font_size(legend_font_size);
					cairo_text_extents_t te;
					cairo_font_extents_t fe;
					cr->select_font_face("Sans", FONT_SLANT_NORMAL, FONT_WEIGHT_BOLD);
					cr->get_font_extents(fe);
					cr->get_text_extents(freq_labels.find(freq)->second, te);
					cr->move_to(this->width - 0.5 * legend_height - te.width / 2 - absthick / 2 - te.x_bearing, legend_height * (index + 2) - legend_height / 2 - fe.descent + fe.height / 2 + absthick / 2);
					setColor(255);
					cr->show_text(freq_labels.find(freq)->second);
				}
			}

			// Restore font size computed above
			this->setFontSize(fontSize);

			// Browse all tasks in schedule to draw them
			map<Task, pair<size_t, size_t> > drawn;
			for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
			{
				for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					// Checks the number of core already covered when drawing this task
					Task task = *j->second.first;
					// done is the number of cores already drawn for the task
					// next is the first core after what has been already drawn for the task
					size_t done = 0, next = 1;
					size_t core = i->first;

					// Find the task in the list of task already partially drawn
					if(drawn.find(task) != drawn.end())
					{
						done = drawn.find(task)->second.first;
						next = drawn.find(task)->second.second;

						// Remove task from list only if it will run again
						if(done < task.getWidth() && core >= next)
						{
							drawn.erase(drawn.find(task));
						}
					}

					// Draw the task only if all its core has not been drawn yet,
					// and if the current core is at least as high as the next allowed
					// core computed last time the task was partially drawn
					if(done < task.getWidth() && core >= next)
					{
						task.setWorkload(tg->getTasks().find(task)->getWorkload());
						task.setMaxWidth(tg->getTasks().find(task)->getMaxWidth());
						task.setEfficiencyString(tg->getTasks().find(task)->getEfficiencyString());
						double runtime = task.runtime(task.getWidth(), task.getFrequency());

						// Looks for the number of cores through the task can be drawn in a continuous manner
						// Instead of the task's width, because mapping may not be to contiguous cores
						size_t width = 0;
						for(Schedule::table::const_iterator k = i; k != sched->getSchedule().end(); k++)
						{
							bool task_found = false;
							for(Schedule::sequence::const_iterator l = k->second.begin(); l != k->second.end(); l++)
							{
								if(*l->second.first == task)
								{
									width++;
									task_found = true;;
								}
							}

							if(!task_found)
							{
								break;
							}
						}
						double start = task.getStartTime();
						string name;
						if(useTaskName)
						{
							name = task.getName();
						}
						else
						{
							stringstream ss;
							ss << (std::distance(sched->getTasks().begin(), sched->getTasks().find(task)) + 1);
							name = ss.str();
						}
						this->drawTask(runtime, width, task.getFrequency(), start, core, name);
						drawn.insert(pair<Task, pair<size_t, size_t> >(task, pair<size_t, size_t>(done + width, core + width)));
					}
				}
			}

			// Draw final deadline, if necessary
			this->drawBarrier(0);
			if(drawDeadline)
			{
				this->drawBarrier(this->height - this->absthick);
			}
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
			//cr->move_to(this->absthick / 2 + this->yaxis_size, height - time * magnify - this->absthick / 2); 
			//cr->line_to(width - this->absthick / 2 - legend_size, height - time * magnify - this->absthick / 2);
			cr->move_to(this->absthick / 2 + this->yaxis_size, height - time - this->absthick / 2); 
			cr->line_to(width - this->absthick / 2 - legend_size, height - time - this->absthick / 2);
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
			this->cr->set_font_size(this->fontSize);
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
		bool drawDeadline;
		bool showFrequencies;
		map<float, unsigned int> colors;
#ifdef CAIRO_HAS_SVG_SURFACE
		Cairo::RefPtr<Cairo::Context> cr;
		Cairo::RefPtr<Cairo::SvgSurface> surface;
#endif
};

void
TetrisSchedule::dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt) const
{
	set<float> frequencies;
	for(set<const Core*>::iterator i = pt->getCores().begin(); i != pt->getCores().end(); i++)
	{
		const Core *core = *i;
		for(set<float>::iterator j = core->getFrequencies().begin(); j != core->getFrequencies().end(); j++)
		{
			frequencies.insert(*j);
		}
	}

	double thickness = 0.02;
	double magnify = 100;

	Schedule::table schedule = sched->getSchedule();

	// Browse all tasks to find the time the last task stops	
	double max_stop_time = 0;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task task = *j->second.first;
			task.setWorkload(tg->getTasks().find(task)->getWorkload());
			task.setMaxWidth(tg->getTasks().find(task)->getMaxWidth());
			task.setEfficiencyString(tg->getTasks().find(task)->getEfficiencyString());
			double stop_time = task.getStartTime() + task.runtime(task.getWidth(), task.getFrequency());
			if(stop_time > max_stop_time)
			{
				max_stop_time = stop_time;
			}
		}
	}

	double deadline = tg->getDeadline(pt);
	bool drawDeadline = deadline > 0;
	deadline = deadline > 0 ? deadline : max_stop_time;
	Canvas(deadline, deadline * ratio, thickness, magnify, makeGradient(colors, frequencies), drawDeadline, this->showFrequencies).dump(os, sched, tg, pt, useTaskName, strokeSize);
}

TetrisSchedule*
TetrisSchedule::clone() const
{
	return new TetrisSchedule();
}

