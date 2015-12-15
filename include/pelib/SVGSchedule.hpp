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


#include <pelib/Schedule.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/SVGScheduleOutput.hpp>

#ifndef PELIB_SVGSCHEDULE_H
#define PELIB_SVGSCHEDULE_H

namespace pelib
{
	/** Parser and output class for pelib::Schedule in SVG format **/
	class SVGSchedule : public SVGScheduleOutput
	{
		public:
			SVGSchedule();
			SVGSchedule(double deadline, float ratio, bool showCores, bool showFrequencies, bool linearFrequencyradiant, unsigned int lowFrequencyRGBA, unsigned int highFrequencyRGBA);
			/** Destructor **/
			virtual ~SVGSchedule();
			/** Writes the content of instance of pelib::Schedule into output stream in SVG format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule *data, const Taskgraph *tg, const Platform *pt) const;
			/** Writes the content of instance of pelib::Schedule into output stream in SVG format, using additional information from associated taskgraph and platform **/
			virtual	void dump(std::ostream &os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const;
			/** Read text input in SVG format and creates an instance of pelib::Schedule containing all data read **/
			virtual SVGSchedule* clone() const;
	};
}

#endif
