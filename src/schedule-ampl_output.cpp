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

#include <pelib/parser.h>
#include <pelib/output.h>
//#include <pelib/argument_parsing.hpp>
//#include <pelib/dl.h>

#include <pelib/AmplOutput.hpp>
#include <pelib/Schedule.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

#if 0
struct parameters {
	char* taskgraph;
};
typedef struct parameters parameters_t;

static pelib_argument_stream_t
parse_args(size_t argc, char **argv)
{
	pelib_argument_stream_t parameters;
	pelib_argument_stream_init(&parameters);

	while(*argv != NULL)
	{
		if(string(*argv).compare("--taskgraph") == 0)
		{
			argv++;
			argv += pelib_argument_stream_parse(argv, &parameters) + 1;
		}

		argv++;
	}

	return parameters;
}
#endif

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv)
{
#if 0
	pelib_argument_stream_t parameters = parse_args(argc, argv);
	Taskgraph *tg;

	/* Load functions from shared libraries */
	void *libParser;
	if(parameters.library != NULL)
	{
		libParser = load_lib(parameters.library);

		/* Link function handles to function pointers */
		Record* (*parse)(istream&, size_t, char**) = (Record* (*)(istream&, size_t, char**))load_function(libParser, "pelib_parse");

		switch(parameters.stream)
		{
			case STREAM_STDIN:
				{
					tg = (Taskgraph*)parse(cin, parameters.argc, parameters.argv);
				}
			break;
			case STREAM_STDOUT:
			case STREAM_STDERR:
				cerr << "[WARNING] Cannot read taskgraph from an output stream to load a schedule. Skipping." << endl;
			break;
			case STREAM_FILE:
				{
					ifstream myfile(parameters.filename);
					tg = (Taskgraph*)parse(myfile, parameters.argc, parameters.argv);
						myfile.close();
					}
				break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No input stream specified for taskgraph input of schedule. Skipping." << endl;
			break;
		}

		destroy_lib(libParser);
	}
	else
	{
		cerr << "[WARNING] No parser library specified for taskgraph input of schedule. Skipping." << endl;
	}


	// We don't use the name parameter so we can destroy the structure now.
	pelib_argument_stream_destroy(parameters);
// We don't care about any argument here
#if 0
	while(*argv != NULL)
	{
		cout << *argv << " ";
		argv++;
	}
	cout << endl;
#endif

//	cout << cin.rdbuf();
#endif
	Algebra al = AmplOutput(AmplOutput::floatHandlers()).parse(cin);
	Schedule *sched = new Schedule("Converted from Algebra", al);
//	delete tg;

	return sched;
}

void
pelib_delete(Record* rec)
{
	delete rec;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Schedule *sc = (Schedule*)records.find(typeid(Schedule).name())->second;
	AmplOutput(AmplOutput::floatHandlers()).dump(cout, sc->buildAlgebra());
}

#ifdef __cplusplus
}
#endif

