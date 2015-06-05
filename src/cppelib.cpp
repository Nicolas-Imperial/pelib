#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

#include <PlatformParser.hpp>
#include <PlatformOutput.hpp>
#include <AmplPlatform.hpp>

#include <pelib/platform_factory.hpp>

using namespace std;
using namespace pelib;

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

static bool
parse_arguments(int argc, char **argv)
{
	// Loop: skip the first argument, loop until we find a null string pointer and increment pointer at each iteration
	for(argv++; *argv != NULL; argv++)
	{
		cout << "argv: \"" << *argv << "\"." << endl;
		if(string(*argv).compare("--taskgraph") == 0)
		{
			record = REC_TASKGRAPH;
			record_string = string("taskgraph");
			continue;
		}

		if(string(*argv).compare("--platform") == 0)
		{
			record = REC_PLATFORM;
			record_string = string("platform");
			continue;
		}
		
		if(string(*argv).compare("--input") == 0)
		{
			// See what format that should be
			argv++;

			if(string(*argv).compare("--shared") == 0)
			{
				argv++;
				input = REF_SHARED;
				input_shared = string(*argv);
				continue;
			}
			
			if(string(*argv).compare("--format") == 0)
			{
				argv++;
				input = REF_FORMAT;
				input_shared = string(*argv);
				continue;
			}
		}

		if(string(*argv).compare("--output") == 0)
		{
			// See what format that should be
			argv++;

			if(string(*argv).compare("--shared") == 0)
			{
				argv++;
				output = REF_SHARED;
				output_shared = string(*argv);
				continue;
			}
			
			if(string(*argv).compare("--format") == 0)
			{
				argv++;
				output = REF_FORMAT;
				output_shared = string(*argv);
				continue;
			}
		}

		if(string(*argv).compare("--platform") == 0)
		{
			// See what format that should be
			for(argv++; platform == REF_INVALID || platform_file.compare("") == 0; argv++)
			{
				if(string(*argv).compare("--shared") == 0)
				{
					argv++;
					platform = REF_SHARED;
					platform_shared = string(*argv);
					continue;
				}
				
				if(string(*argv).compare("--format") == 0)
				{
					argv++;
					platform = REF_FORMAT;
					platform_shared = string(*argv);
					continue;
				}

				if(string(*argv).compare("--file") == 0)
				{
					argv++;
					platform_file = string(*argv);
					continue;
				}

				// Not a platform option
				break;
			}
		}

		// Notify an error
		cerr << "[WARNING] Incorrect argument: \"" << *argv << "\". Ignoring" << endl;
	}

	// Chase errors
	if(record == REC_INVALID)
	{
		cerr << "[ ERROR ] No conversion type specified (--record, --taskgraph, --platform or --schedule). Abandoning" << endl;
		return false;
	}

	if(input == REF_INVALID || input_shared.compare("") == 0)
	{
		cerr << "[ ERROR ] No input parser format or library given (ex: --shared libtaskgraph-graphml.so or --format graphml). Abandoning." << endl;
		return false;
	}
	
	if(output == REF_INVALID || output_shared.compare("") == 0)
	{
		cerr << "[ ERROR ] No output parser format or library given (ex: --shared libtaskgraph-graphml.so or --format graphml). Abandoning." << endl;
		return false;
	}
	
	// Arrange arguments collected
	if(input == REF_FORMAT)
	{	
		input_shared = string("lib").append(record_string).append(string("-")).append(input_shared).append(string(".so"));
	}

	// Arrange arguments collected
	if(output == REF_FORMAT)
	{	
		output_shared = string("lib").append(record_string).append(string("-")).append(output_shared).append(string(".so"));
	}

	// Arrange arguments collected
	if(platform == REF_FORMAT)
	{	
		platform_shared = string("lib").append(record_string).append(string("-")).append(platform_shared).append(string(".so"));
	}

	return true;
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
	if(parse_arguments(argc, argv))
	{
		cout << "Using input library \"" << input_shared << "\"." << endl;
		cout << "Using output library \"" << output_shared << "\"." << endl;

		switch(record)
		{
			case REC_RECORD:
			break;
			case REC_TASKGRAPH:
			break;
			case REC_PLATFORM:
			{
				/* Load functions from shared libraries */
				void *libParser, *libOutput;
				libParser = load_lib(input_shared.c_str());
				libOutput = load_lib(output_shared.c_str());

				/* Link function handles to function pointers */
				PlatformParser* (*create_parser)() = (PlatformParser* (*)())load_function(libParser, "create_parser");
				void (*delete_parser)(PlatformParser*) = (void(*)(PlatformParser*))load_function(libParser, "delete_parser");
				PlatformOutput* (*create_output)() = (PlatformOutput* (*)())load_function(libOutput, "create_output");
				void (*delete_output)(PlatformOutput*) = (void(*)(PlatformOutput*))load_function(libOutput, "delete_output");
				PlatformParser *parser = create_parser();
				Platform *arch = parser->parse(cin);
				delete_parser(parser);

				PlatformOutput *output = create_output();
				output->dump(cout, arch);
				delete_output(output);

				destroy_lib(libParser);
				destroy_lib(libOutput);
		
				// Delete record
				delete arch;
			}
			break;
			case REC_SCHEDULE:
			break;
			case REC_INVALID:
			default:
				cerr << "[ ERROR ] No conversion type specified (--record, --taskgraph, --platform or --schedule). Abandoning" << endl;
			break;
		}

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}
