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

#include <pelib/Record.hpp>

#include <pelib/argument_parsing.hpp>
#include <pelib/dl.h>

#ifdef debug
#undef debug
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

using namespace std;
using namespace pelib;

struct conversion
{
	vector<pelib_argument_stream_t> inputs;
	pelib_argument_stream_t process;
	vector<pelib_argument_stream_t> outputs;
};
typedef struct conversion conversion_t;

static conversion_t
parse_args(char** argv)
{
	conversion_t conversion;
	string self(argv[0]);
	conversion.process.library = NULL;

	for(argv++; *argv != NULL; argv++)
	{
		if(string(*argv).compare("--input") == 0)
		{
			pelib_argument_stream_t input;
			pelib_argument_stream_init(&input);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &input);
			argv = argv + num - 1;
			conversion.inputs.push_back(input);
			continue;
		}

		if(string(*argv).compare("--process") == 0)
		{
			pelib_argument_stream_t process;
			pelib_argument_stream_init(&process);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &process);
			argv = argv + num - 1;
			conversion.process = process;
			continue;
		}

		if(string(*argv).compare("--output") == 0)
		{
			pelib_argument_stream_t output;
			pelib_argument_stream_init(&output);
			argv++;
			size_t num = pelib_argument_stream_parse(argv, &output);
			argv = argv + num - 1;
			conversion.outputs.push_back(output);
			continue;
		}

		if(string(*argv).compare("--sources") == 0)
		{
			string cmd = string("ls $(dirname $(realpath $(which ").append(self).append(")))/../share/pelib/pelib-*.tar.gz | sort -rV | head -1 | xargs realpath");
			exit(system(cmd.c_str()));
		}

		cerr << "[WARNING] Incorrect argument: \"" << *argv << "\". Ignoring" << endl;
	}

	return conversion;
}

int
main(int argc, char **argv)
{
	void *libProcess = 0;
	map<const char*, Record*> inputs;
	map<const char*, void (*)(Record*)> freelist;
	map<const char*, void*> parserList;

	conversion_t conversion = parse_args(argv);

	size_t counter = 0;
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++, counter++)
	{
		/* Load functions from shared libraries */
		void *libParser;
		if(i->library != NULL)
		{
			libParser = load_lib(i->library);
		}
		else
		{
			cerr << "[WARNING] No parser library specified for input #" << counter << ". Skipping." << endl;
			continue;
		}

		/* Link function handles to function pointers */
		Record* (*parse)(istream&, size_t, char**) = (Record* (*)(istream&, size_t, char**))load_function(libParser, "pelib_parse");
		void (*del)(Record*) = (void (*)(Record*))load_function(libParser, "pelib_delete");

		switch(i->stream)
		{
			case STREAM_STDIN:
				{
					Record* rec = parse(cin, i->argc, i->argv);
					if(rec != NULL)
					{
						if(i->name != NULL)
						{
							inputs.insert(pair<const char*, Record*>(i->name, rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(i->name, del));
							parserList.insert(pair<const char*, void*>(pair<const char*, void*>(i->name, libParser)));
						}
						else
						{
							inputs.insert(pair<const char*, Record*>(typeid(*rec).name(), rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(typeid(*rec).name(), del));
							parserList.insert(pair<const char*, void*>(pair<const char*, void*>(typeid(*rec).name(), libParser)));
						}
					}
					else
					{
						cerr << "[WARNING] Parser \"" << i->library << "\" failed to parse input on stdin for input #" << counter << ". Skipping." << endl;
					}
				}
			break;
			case STREAM_STDOUT:
			case STREAM_STDERR:
				cerr << "[WARNING] Cannot read data from an output stream for input #" << counter << ". Skipping." << endl;
			break;
			case STREAM_FILE:
				{
					ifstream myfile(i->filename);
					Record *rec = parse(myfile, i->argc, i->argv);
					
					if(rec != NULL)
					{
						if(i->name != NULL)
						{
							inputs.insert(pair<const char*, Record*>(i->name, rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(i->name, del));
						}
						else
						{
							inputs.insert(pair<const char*, Record*>(typeid(*rec).name(), rec));
							freelist.insert(pair<const char*, void (*)(Record*)>(typeid(*rec).name(), del));
						}
						myfile.close();
					}
					else
					{
						cerr << "[WARNING] Parser \"" << i->library << "\" failed to parse input on stdin for input #" << counter << ". Skipping." << endl;
					}
				}
			break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No input stream specified for input #" << counter << ". Skipping." << endl;
			break;
		}

		// Don't destroy the libparser yet, as we need to destroy data elements parsed
		// Don't destroy the pelib_argument_stream descriptor yet as we still need input names
	}

	if(conversion.process.library != NULL)
	{
		libProcess = load_lib(conversion.process.library);

		/* Link function handles to function pointers */
		std::map<const char*, Record*> (*process)(std::map<const char*, Record*> records, size_t, char**) = (std::map<const char*, Record*> (*)(std::map<const char*, Record*> records, size_t, char**))load_function(libProcess, "pelib_process");
		std::map<const char*, Record*> transform = process(inputs, conversion.process.argc, conversion.process.argv);

		// Free data structure parsed
		for(map<const char*, void (*)(Record*)>::iterator i = freelist.begin(); i != freelist.end(); i++)
		{
			void (*del)(Record*) = i->second;
			del(inputs.find(i->first)->second);
		}

		// Replace inputs with transformation outcome
		inputs = transform;

		// Replace function pointers
		freelist.clear();
		void (*del)(Record*) = (void (*)(Record*))load_function(libProcess, "pelib_delete");
		for(map<const char*, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
		{
			freelist.insert(pair<const char*, void (*)(Record*)>(i->first, del));
		}
	}

	counter = 0;
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.outputs.begin(); i != conversion.outputs.end() && inputs.size() > 0; i++, counter++)
	{
		/* Load functions from shared libraries */
		void *libOutput;
		if(i->library != NULL)
		{
			libOutput = load_lib(i->library);
		}
		else
		{
			cerr << "[WARNING] No parser library specified for output #" << counter << ". Skipping." << endl;
			continue;
		}

		/* Link function handles to function pointers */
		void (*dump)(ostream&, std::map<const char*, Record*> records, size_t, char**) = (void (*)(ostream&, std::map<const char*, Record*> records, size_t, char**))load_function(libOutput, "pelib_dump");

		switch(i->stream)
		{
			case STREAM_STDIN:
				cerr << "[WARNING] Cannot dump data to an input stream for output #" << counter << ". Skipping." << endl;
			case STREAM_STDOUT:
				dump(cout, inputs, i->argc, i->argv);
			break;
			case STREAM_STDERR:
				dump(cerr, inputs, i->argc, i->argv);
			break;
			case STREAM_FILE:
				{
					ofstream myfile(i->filename, ios::out | ios::trunc | ios::binary);
					dump(myfile, inputs, i->argc, i->argv);
					myfile.close();
				}
			break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No output stream specified for output #" << counter << ". Skipping." << endl;
			break;
		}

		// Destroy dynamic libraries
		destroy_lib(libOutput);

		// Destroy the stream descriptor
		pelib_argument_stream_destroy(*i);
	}

	// Destroy input structures
	for(map<const char*, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
	{
		void (*del)(Record*) = freelist.find(i->first)->second;
		del(i->second);
	}

	// Destroy input library handlers
	for(map<const char*, void*>::iterator i = parserList.begin(); i != parserList.end(); i++)
	{
		destroy_lib(i->second);
	}

	// Delete processing dynamic library
	if(conversion.process.library != NULL)
	{
		destroy_lib(libProcess);
	}

	// Destroy input arguments
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++)
	{
		pelib_argument_stream_destroy(*i);
	}
}
