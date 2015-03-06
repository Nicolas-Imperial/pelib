#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>
#include <Platform.hpp>
#include <AmplPlatform.hpp>
#include <GraphML.hpp>

using namespace std;
using namespace pelib;

enum action {NONE, NAME, TASKS, TASK};

enum action request = NONE;
int input_stdin = 0;
char* input_file = NULL;
int task = 0;

#if DEBUG
#define trace(var) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"." << endl
#else
#define trace(var)
#endif

static void
set_action(enum action req)
{
	if(request != NONE)
	{
		cerr << "[WARN ] Action already set, replacing." << endl;
	}
	request = req;
}

void
read_args(char **argv)
{
	char * strend;
	while(*argv != NULL)
	{
//		cerr << "[DEBUG] argv = \"" << *argv << "\"." << endl;
//		cerr << "[DEBUG] strcmp(*argv, \"--tasks\") == \"" << strcmp(*argv, "--tasks") << "\"." << endl;
		if(!strcmp(*argv, "--read") || !strcmp(*argv, "-r"))
		{
			char *opt = *argv;
			argv++;

			if(!strcmp(*argv, "name"))
			{
				set_action(NAME);
			}
			else if(!strcmp(*argv, "tasks"))
			{
				set_action(TASKS);
			}
			else if(!strcmp(*argv, "task"))
			{
				set_action(TASK);

				argv++;
				if(**argv != '-')
				{
					task = strtol(*argv, &strend, 10);

					if((size_t)strend - (size_t)(*argv) < strlen(*argv) || task <= 0)
					{
						cerr << "[WARN ] Invalid Missing task ID provided (\"" << *argv << "\"). Aborting." << endl;
						exit(1);
					}
				}
				else
				{
					cerr << "[WARN ] Missing task name to read information from. Ignoring \"" << opt << "\" directive." << endl;
					argv--;
				}
			}
			else
			{
				cerr << "[WARN ] Invalid field for read action: \"" << argv << "\". Ignoring \"" << opt << "\" directive." << endl;
			}
		}
		else if(!strcmp(*argv, "--input") || !strcmp(*argv, "-i"))
		{
			// Get to next parameter
			char *opt = *argv;
			argv++;

			if(!strcmp(*argv, "-"))
			{
				input_stdin = 1;
			} else
			{
				if(**argv != '-')
				{
					input_file = *argv;
				}
				else
				{
					cerr << "[WARN ] Invalid value \"" << argv << "\" for option \"" << opt << "\". Ignoring \"" << opt << "\" directive." << endl;
					argv--;
				}
			}
		}

		argv++;
	}
}

int main(int argc, char **argv)
{
	read_args(argv);
	
	GraphML input;
	Taskgraph *tg;
	if(input_stdin != 0)
	{
		tg = input.parse(cin);
	}
	else
	{
		if(input_file != NULL)
		{
			std::ifstream myfile;
			myfile.open (input_file, std::ios::in);
			tg = input.parse(myfile);
			myfile.close();
		}
		else
		{
			cerr << "[ERROR] No input to read  taskgraph from. Aborting." << endl;
			exit(1);
		}
	}

	switch(request)
	{
		case NAME:
			cout << tg->getAUTName() << endl;	
		break;
		case TASKS:
			for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
			{
				cout << i->getId() << " ";
			}
			cout << endl;
		break;
		case TASK:
			cout << tg->findTask(task).getName() << endl;
			
			// This is tougher
		break;
		case NONE:
			cerr << "[ERROR] No valid action requested. Aborting." << endl;
			exit(1);
		break;
		default:
			cerr << "[ERROR] Unknown action requested. Aborting." << endl;
			exit(1);
		break;
	}
		
	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
		<< std::setiosflags(std::ios::fixed)                                                                                                     
		<< std::setiosflags(std::ios::showpoint);  

	return 0;
}
