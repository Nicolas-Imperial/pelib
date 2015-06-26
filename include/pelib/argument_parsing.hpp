#ifndef PELIB_ARGUMENT_PARSING_H
#define PELIB_ARGUMENT_PARSING_H

enum stream {STREAM_NOTHING, STREAM_STDIN, STREAM_STDOUT, STREAM_STDERR, STREAM_FILE};

struct pelib_argument_stream
{
	char *library;
	char **argv;
	size_t argc;
	char *filename;
	char *name;
	enum stream stream;
};
typedef struct pelib_argument_stream pelib_argument_stream_t;

void pelib_argument_stream_init(pelib_argument_stream_t *pelib_argument_stream);

void pelib_argument_stream_destroy(const pelib_argument_stream_t pelib_argument_stream);

unsigned int
pelib_argument_stream_parse(char **argv, pelib_argument_stream_t* stream);

#endif

