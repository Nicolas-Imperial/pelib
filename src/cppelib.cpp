#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

#include <PlatformParser.hpp>
#include <PlatformOutput.hpp>
#include <AmplPlatform.hpp>

//#include <pelib/platform_factory.hpp>

using namespace std;
using namespace pelib;

#if 0
enum record {REC_INVALID, REC_RECORD, REC_TASKGRAPH, REC_PLATFORM, REC_SCHEDULE};
enum libref {REF_INVALID, REF_SHARED, REF_FORMAT};

enum record record = REC_INVALID;
enum libref input = REF_INVALID, output = REF_INVALID, platform = REF_INVALID;
//bool input_shared_ = false;
string input_shared = "";
string output_shared = "";
string platform_shared = "";
string record_string = "";
string platform_file = "";
#endif

enum stream {STREAM_NOTHING, STREAM_STDIN, STREAM_STDOUT, STREAM_STDERR, STREAM_FILE};

struct datastream
{
	char *library;
	char **argv;
	size_t argc;
	char *filename;
	enum stream stream;
};
typedef struct datastream datastream_t;

struct conversion
{
	vector<datastream_t> inputs;
	vector<datastream_t> outputs;
};
typedef struct conversion conversion_t;

static void
datastream_init(datastream_t *datastream)
{
	datastream->library = NULL;
	datastream->argv = (char**)malloc(sizeof(char*) * 1);
	datastream->argv[0] = NULL;
	datastream->argc = 0;
	datastream->filename = NULL;
	datastream->stream = STREAM_NOTHING;
}

static void
datastream_destroy(const datastream_t& datastream)
{
	free(datastream.argv);

	if(datastream.library != NULL)
	{
		free(datastream.library);
	}

	if(datastream.filename != NULL)
	{
		free(datastream.filename);
	}
}

static bool
is_reserved(char* str)
{
	// Anything starting with a dash (-) is an option name and invalid as an option argument
	if(str == NULL || string(str).compare("-") == 0)
	{
		return true;
	}
	
	// Nothing wrong was found with this option argument
	return false;
}

static void
add_argument(datastream *stream, char *argv)
{
			// Make room for another argument
			char **stuff = (char**)malloc(sizeof(char*) * (stream->argc + 2));
			memcpy(stuff, stream->argv, (stream->argc + 1) * sizeof(char*));
			free(stream->argv);
			stream->argv = stuff;

			stream->argv[stream->argc] = (char*)malloc(sizeof(char) * (strlen(argv) + 1));
			strcpy(stream->argv[stream->argc], argv);
			stream->argc++;
}

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

static unsigned int
parse_stream(char **argv, datastream_t* stream)
{
	unsigned int parsed = 0;
	while(*argv != NULL)
	{
		if(string(*argv).compare("--lib") == 0 || string(*argv).compare("-l") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				stream->library = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->library, *argv);
				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--class") == 0 || string(*argv).compare("-c") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				#define LIBPELIB "libpelib-"
				#define SO ".so"
				stream->library = *argv;
				stream->library = (char*)malloc(sizeof(char) * (strlen(LIBPELIB) + strlen(*argv) + strlen(SO) + 1));
				strcpy(stream->library, LIBPELIB);
				strcpy(stream->library + strlen(LIBPELIB), *argv);
				strcpy(stream->library + strlen(LIBPELIB) + strlen(*argv), SO);

				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--file") == 0 || string(*argv).compare("-f") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				stream->stream = STREAM_FILE;
				stream->filename = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->filename, *argv);
				argv++;
				parsed++;
			}
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stdin") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDIN;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stdout") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDOUT;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--stderr") == 0)
		{
			argv++;
			parsed++;
			stream->stream = STREAM_STDERR;
			
			// proceed with next argument
			continue;
		}

		if(string(*argv).compare("--args") == 0)
		{
			argv++;
			parsed++;
	
			while(*argv != NULL && string(*argv).compare("--") != 0)
			{
				add_argument(stream, *argv);
				argv++;
				parsed++;
			}
			
			// Skip the last "--"
			if(*argv != NULL)
			{
				if(string(*argv).compare("--") == 0)
				{
					argv++;
					parsed++;	
				}
			}

			continue;
		}

		if(string(*argv).compare("-o") == 0)
		{
			argv++;
			parsed++;

			add_argument(stream, *argv);
			argv++;
			parsed++;

			continue;
		}
		
		// This is not a valid option; exit now
		return parsed;
	}

	// Not parsed anything
	return parsed;
}

static conversion_t
parse_args(char** argv)
{
	conversion_t conversion;

	for(argv++; *argv != NULL; argv++)
	{
		if(string(*argv).compare("--input") == 0)
		{
			datastream_t input;
			datastream_init(&input);
			argv++;
			size_t num = parse_stream(argv, &input);
			argv = argv + num - 1;
			conversion.inputs.push_back(input);
			continue;
		}

		if(string(*argv).compare("--output") == 0)
		{
			datastream_t output;
			datastream_init(&output);
			argv++;
			size_t num = parse_stream(argv, &output);
			argv = argv + num - 1;
			conversion.outputs.push_back(output);
			continue;
		}

		cerr << "[WARNING] Incorrect argument: \"" << *argv << "\". Ignoring" << endl;
	}

	return conversion;
}

static void*
load_lib(const char* library)
{
        void *handle;
        char *error;

        handle = dlopen(library, RTLD_LAZY);
        if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
                exit(EXIT_FAILURE);
        }

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "%s\n", error);
                exit(EXIT_FAILURE);
        }   

        return handle;
}

static void*
load_function(void *handle, const char *function)
{
        dlerror();    /* Clear any existing error */

        void *func = (void*)dlsym(handle, function);
        char *error;

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "%s\n", error);
                exit(EXIT_FAILURE);
        }   

        return func;
}

static void
destroy_lib(void *handle)
{
        dlclose(handle);
}

int
main(int argc, char **argv)
{
	conversion_t conversion = parse_args(argv);

	map<char*, Record*> inputs;
	size_t counter = 0;
	for(vector<datastream_t>::const_iterator i = conversion.inputs.begin(); i != conversion.inputs.end(); i++)
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
					Record *rec = parse(cin, i->argc, i->argv);
					inputs.insert(pair<char*, Record*>((char*)typeid(rec).name(), rec));
				}
			break;
			case STREAM_STDOUT:
			case STREAM_STDERR:
				cerr << "[WARNING] Cannot read data from an output stream for input #" << counter << ". Skipping." << endl;
			break;
			case STREAM_FILE:
				{
					ifstream myfile(i->filename, ios::binary);
					Record *rec = parse(myfile, i->argc, i->argv);
					inputs.insert(pair<char*, Record*>((char*)typeid(rec).name(), rec));
					myfile.close();
				}
			break;
			case STREAM_NOTHING:
			default:
				cerr << "[WARNING] No input stream specified for input #" << counter << ". Skipping." << endl;
			break;
		}

		destroy_lib(libParser);

		// Destroy the stream descriptor
		datastream_destroy(*i);

		// Count the number of parser being processed
		counter++;
	}

	// Free input fields
	for(map<char*, Record*>::iterator i = inputs.begin(); i != inputs.end(); i++)
	{
		delete i->second;
	}
}
