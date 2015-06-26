#include <cstdlib>
#include <string>
#include <iostream>

#include <string.h>

#include <pelib/argument_parsing.hpp>

using namespace std;

void
pelib_argument_stream_init(pelib_argument_stream_t *pelib_argument_stream)
{
	pelib_argument_stream->library = NULL;
	pelib_argument_stream->argv = (char**)malloc(sizeof(char*) * 1);
	pelib_argument_stream->argv[0] = NULL;
	pelib_argument_stream->argc = 0;
	pelib_argument_stream->filename = NULL;
	pelib_argument_stream->name = NULL;
	pelib_argument_stream->stream = STREAM_NOTHING;
}

void
pelib_argument_stream_destroy(const pelib_argument_stream_t pelib_argument_stream)
{
	free(pelib_argument_stream.argv);

	if(pelib_argument_stream.library != NULL)
	{
		free(pelib_argument_stream.library);
	}

	if(pelib_argument_stream.filename != NULL)
	{
		free(pelib_argument_stream.filename);
	}

	if(pelib_argument_stream.name != NULL)
	{
		free(pelib_argument_stream.name);
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
add_argument(pelib_argument_stream *stream, char *argv)
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

unsigned int
pelib_argument_stream_parse(char **argv, pelib_argument_stream_t* stream)
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

		if(string(*argv).compare("--format") == 0 || string(*argv).compare("-m") == 0)
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

		if(string(*argv).compare("--name") == 0 || string(*argv).compare("-n") == 0)
		{
			argv++;
			parsed++;
			
			if(is_reserved(*argv))
			{
				return parsed;
			}
			else
			{
				debug(*argv);
				stream->name = (char*)malloc(sizeof(char) * (strlen(*argv) + 1));
				strcpy(stream->name, *argv);
				debug(stream->name);

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

