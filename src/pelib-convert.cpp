#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

#include <pelib/Record.hpp>

#include <pelib/argument_parsing.hpp>
#include <pelib/dl.h>

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

using namespace std;
using namespace pelib;

struct conversion
{
	vector<pelib_argument_stream_t> inputs;
	vector<pelib_argument_stream_t> outputs;
};
typedef struct conversion conversion_t;

static conversion_t
parse_args(char** argv)
{
	conversion_t conversion;

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

		cerr << "[WARNING] Incorrect argument: \"" << *argv << "\". Ignoring" << endl;
	}

	return conversion;
}

int
main(int argc, char **argv)
{
	conversion_t conversion = parse_args(argv);

	map<const char*, Record*> inputs;
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
						}
						else
						{
							inputs.insert(pair<const char*, Record*>(typeid(*rec).name(), rec));
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
						}
						else
						{
							inputs.insert(pair<const char*, Record*>(typeid(*rec).name(), rec));
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

		destroy_lib(libParser);

		// Don̈́'t destroy the pelib_argument_stream descriptor yet as we still need input names
	}

	counter = 0;
	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.outputs.begin(); i != conversion.outputs.end() && inputs.size() > 0; i++, counter++)
	{
		/* Load functions from shared libraries */
		void *libParser;
		if(i->library != NULL)
		{
			libParser = load_lib(i->library);
		}
		else
		{
			cerr << "[WARNING] No parser library specified for output #" << counter << ". Skipping." << endl;
			continue;
		}

		/* Link function handles to function pointers */
		void (*dump)(ostream&, std::map<const char*, Record*> records, size_t, char**) = (void (*)(ostream&, std::map<const char*, Record*> records, size_t, char**))load_function(libParser, "pelib_dump");

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

		destroy_lib(libParser);

		// Destroy the stream descriptor
		pelib_argument_stream_destroy(*i);
	}

	for(vector<pelib_argument_stream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++)
	{
		pelib_argument_stream_destroy(*i);
	}

	// Free input fields
	for(map<const char*, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
	{
		delete i->second;
	}
}
