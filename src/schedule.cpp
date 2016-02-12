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


#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

#include <pelib/Taskgraph.hpp>
#include <pelib/Platform.hpp>
#include <pelib/Schedule.hpp>

#include <pelib/XMLSchedule.hpp>

#include <pelib/process.h>
#include <pelib/argument_parsing.hpp>
#include <pelib/dl.h>

#ifdef debug
#undef debug
#endif

#if 01
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#else
#define(expr)
#endif

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

struct args
{
	bool showStatsOut;
	bool showStatsErr;
	char* showStatsFile;
	size_t parsed;
};
typedef struct args args_t;

static
args_t
parse(char** arg)
{
	args_t args;
	args.showStatsOut = false;
	args.showStatsErr = false;
	args.showStatsFile = NULL;
	args.parsed = 0;

	for(; arg[0] != NULL; arg++)
	{
		if(strcmp(arg[0], "--show-stats") == 0)
		{
			arg++;
			args.parsed += 2;
			
			if(strcmp(arg[0], "--stdout") == 0)
			{
				args.showStatsOut = true;
			}
			else if(strcmp(arg[0], "--stderr") == 0)
			{
				args.showStatsErr = true;
			}
			else
			{
				args.showStatsFile = arg[0];
			}
			continue;
		}
	}

	return args;
}

// TODO: find a way to use libdl to invoke the library that created the object
char *library;

std::map<const char*, pelib::Record*>
pelib_process(std::map<const char*, pelib::Record*> records, size_t argc, char** argv)
{
	// Parse extra arguments and skip them
	args_t args = parse(argv);
	argv += args.parsed;
	argc -= args.parsed;

	pelib_argument_stream_t conversion;
	pelib_argument_stream_init(&conversion);
	pelib_argument_stream_parse(argv, &conversion);

	if(conversion.library == NULL)
	{
		cerr << "[ERROR] No scheduler specified. Aborting" << endl;
		return map<const char*, Record*>();
	}

	// Load library
	library = conversion.library;
	void *libScheduler = load_lib(library);
	Schedule* (*schedule)(const Taskgraph &tg, const Platform &pt, size_t argc, char **argv, map<const string, double> &statistics) = (Schedule* (*)(const Taskgraph &tg, const Platform &pt, size_t argc, char **argv, map<const string, double> &statistics))load_function(libScheduler, "pelib_schedule");

	// Read input data
	Taskgraph *tg = (Taskgraph*)records.find(typeid(Taskgraph).name())->second;
	if(tg == NULL)
	{
		cerr << "[ERROR] Missing taskgraph description. Aborting" << endl;
		return map<const char*, Record*>();
	}	
	Platform *pt = (Platform*)records.find(typeid(Platform).name())->second;
	if(pt == NULL)
	{
		cerr << "[ERROR] Missing platform description. Aborting" << endl;
		return map<const char*, Record*>();
	}

	// Prepare output collection and fill it with schedule generated by library
	map<const char*, Record*> output;
	map<const string, double> statistics;
	Schedule *sched = schedule(*tg, *pt, conversion.argc, conversion.argv, statistics);
	Taskgraph *ptg = new Taskgraph(tg);
	Platform *ppt = new Platform(pt);
	output.insert(pair<const char*, Record*>(typeid(Schedule).name(), sched));
	output.insert(pair<const char*, Record*>(typeid(Taskgraph).name(), ptg));
	output.insert(pair<const char*, Record*>(typeid(Platform).name(), ppt));

	// Show solving statistics
	if(args.showStatsFile != NULL)
	{
		ofstream statsFileStr(args.showStatsFile);
		for(map<const string, double>::iterator i = statistics.begin(); i != statistics.end(); i++)
		{
			statsFileStr << i->first << " = " << i->second << endl;
		}
		statsFileStr.close();
	}
	else if(args.showStatsOut || args.showStatsErr)
	{
		for(map<const string, double>::iterator i = statistics.begin(); i != statistics.end(); i++)
		{
			if(args.showStatsOut)
			{
				cout << i->first << " = " << i->second << endl;
			}
			else
			{
				cerr << i->first << " = " << i->second << endl;
			}
		}
	}

	destroy_lib(libScheduler);

	return output;
}

void
pelib_delete(pelib::Record* obj)
{
	void *libScheduler = load_lib(library);
	void (*del)(const Schedule*) = (void (*)(const Schedule*))load_function(libScheduler, "pelib_delete");

	if(string(typeid(Schedule).name()).compare(typeid(*obj).name()) == 0)
	{
		del((Schedule*)obj);
	}
	else
	{
		delete obj;
	}
}

#ifdef __cplusplus
}
#endif
