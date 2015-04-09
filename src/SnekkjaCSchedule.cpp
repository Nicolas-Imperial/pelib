#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

extern "C"{
#include <igraph.h>
}

#include "Schedule.hpp"
#include "SnekkjaCSchedule.hpp"

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <Task.hpp>

#include <CastException.hpp>
#include <ParseException.hpp>

using namespace pelib;
using namespace std;

void
SnekkjaCSchedule::dump(ostream& os, const StreamingAppData &data) const
{
	dump(os, &data);
}

void
SnekkjaCSchedule::dump(ostream& os, const StreamingAppData *data) const
{
	os << "\
#include <snekkja/schedule.h> \n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 0\n\
#include <snekkja.h>\n\
#define DONE_merge0 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 0\n\
#include <snekkja.h>\n\
#define DONE_merge0 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 1\n\
#include <snekkja.h>\n\
#define DONE_merge1 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 2\n\
#include <snekkja.h>\n\
#define DONE_merge2 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 3\n\
#include <snekkja.h>\n\
#define DONE_merge3 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 4\n\
#include <snekkja.h>\n\
#define DONE_merge4 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 5\n\
#include <snekkja.h>\n\
#define DONE_merge5 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 6\n\
#include <snekkja.h>\n\
#define DONE_merge6 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 7\n\
#include <snekkja.h>\n\
#define DONE_merge7 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 8\n\
#include <snekkja.h>\n\
#define DONE_merge8 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 9\n\
#include <snekkja.h>\n\
#define DONE_merge9 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 10\n\
#include <snekkja.h>\n\
#define DONE_merge10 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 11\n\
#include <snekkja.h>\n\
#define DONE_merge11 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 12\n\
#include <snekkja.h>\n\
#define DONE_merge12 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 13\n\
#include <snekkja.h>\n\
#define DONE_merge13 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 14\n\
#include <snekkja.h>\n\
#define DONE_merge14 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 15\n\
#include <snekkja.h>\n\
#define DONE_merge15 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 16\n\
#include <snekkja.h>\n\
#define DONE_merge16 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 17\n\
#include <snekkja.h>\n\
#define DONE_merge17 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 18\n\
#include <snekkja.h>\n\
#define DONE_merge18 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 19\n\
#include <snekkja.h>\n\
#define DONE_merge19 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 20\n\
#include <snekkja.h>\n\
#define DONE_merge20 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 21\n\
#include <snekkja.h>\n\
#define DONE_merge21 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 22\n\
#include <snekkja.h>\n\
#define DONE_merge22 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 23\n\
#include <snekkja.h>\n\
#define DONE_merge23 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 24\n\
#include <snekkja.h>\n\
#define DONE_merge24 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 25\n\
#include <snekkja.h>\n\
#define DONE_merge25 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 26\n\
#include <snekkja.h>\n\
#define DONE_merge26 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 27\n\
#include <snekkja.h>\n\
#define DONE_merge27 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 28\n\
#include <snekkja.h>\n\
#define DONE_merge28 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 29\n\
#include <snekkja.h>\n\
#define DONE_merge29 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 30\n\
#include <snekkja.h>\n\
#define DONE_merge30 1\n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 31\n\
#include <snekkja.h>\n\
#define DONE_merge31 1\n\
\n\
size_t _snekkja_p = 6;\n\
size_t _snekkja_tasks_int_core[6] = {1, 2, 4, 8, 16, 32};\n\
snekkja_schedule_task_t snekkja_schedule[6][16] = {{{1, 0, 1}}, {{2,0,1}, {3,1,1}}, {{4,0,1}, {5,1,1}, {6,2,1}, {7,3,1}}, {{8,0,1}, {9,1,1}, {10,2,1}, {11,3,1}, {12,4,1}, {13,5,1}, {14,6,1}, {15,7,1}}, {{16,0,1},{17,1,1},{18,2,1},{19,3,1},{20,4,1},{21,5,1},{22,6,1},{23,7,1},{24,8,1},{25,9,1},{26,10,1},{27,11,1},{28,12,1},{29,13,1},{30,14,1},{31,15,1}}};\n\
\n\
void*\n\
snekkja_execute(size_t id, task_status_t status)\n\
{\n\
	switch(id)\n\
	{\n\
		case 0:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return &snekkja_init(merge, 0);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 0);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 0);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 0);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 1:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 1);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 1);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 1);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 1);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 2:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 2);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 2);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 3);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 3);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 3:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 3);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 3);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 3);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 3);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 4:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 4);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 4);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 4);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 4);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 5:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 5);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 5);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 5);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 5);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 6:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 6);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 6);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 6);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 6);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 7:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 7);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 7);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 7);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 7);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 8:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 8);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 8);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 8);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 8);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 9:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 9);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 9);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 9);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 9);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 10:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 10);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 10);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 10);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 10);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 11:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 11);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 11);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 11);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 11);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 12:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 12);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 12);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 12);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 12);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 13:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 13);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 13);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 13);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 13);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 14:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 14);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 14);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 14);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 14);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 15:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 15);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 15);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 15);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 15);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 16:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 16);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 16);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 16);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 16);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 17:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 17);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 17);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 17);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 17);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 18:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 18);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 18);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 18);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 18);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 19:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 19);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 19);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 19);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 19);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 20:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 20);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 20);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 20);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 20);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 21:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 21);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 21);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 21);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 21);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 22:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 22);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 22);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 22);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 22);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 23:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 23);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 23);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 23);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 23);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 24:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 24);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 24);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 24);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 24);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 25:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 25);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 25);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 25);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 25);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 26:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 26);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 26);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 26);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 26);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 27:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 27);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 27);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 27);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 27);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 28:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 28);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 28);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 28);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 28);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 29:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 29);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 29);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 29);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 29);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 30:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 30);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 30);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 30);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 30);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		case 31:\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(merge, 31);\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(merge, 31);\n\
				break;\n\
				case TASK_CHECK:\n\
					return 0;\n\
				break;\n\
				case TASK_WORK:\n\
					return 0;\n\
				break;\n\
				case TASK_PUSH:\n\
					return 0;\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(merge, 31);\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(merge, 31);\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n\
		default:\n\
			return 0;\n\
		break;\n\
	}\n\
\n\
	return 0;\n\
}\n\
";
	/* TODO: implement
	const Schedule *sched = dynamic_cast<const Schedule* >(data);
	if(sched == NULL) throw CastException("parameter \"data\" was not of type \"Schedule*\".");

	map<int, std::map<float, Task> > schedule = sched->getSchedule();
	float target_makespan = sched->getRoundTime();

	set<string> task_ids;
	
	for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.getTaskId());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<schedule name=\"" << sched->getName() << "\" autname=\"" << sched->getAUTName() << "\" "
		<< "roundtime=\"" << target_makespan << "\" "
		<< "cores=\"" << schedule.size() << "\" "
		<< "tasks=\"" << task_ids.size() << "\""
		<< "> \n";

	for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p - 1 << "\">\n";
		std::map<float, Task> core_schedule = i->second;
		int order = 0;

		for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			string taskid = j->second.getTaskId();
			os << "   <task taskid=\"" << taskid << "\" "
				<< "starting_time=\"" << j->second.getStartTime() << "\" "
				<< "frequency=\"" << j->second.getFrequency() << "\" "
				<< "width=\"" << j->second.getWidth() << "\" "
				<< "workload=\"" << j->second.getWorkload() << "\" "
				<< "/>\n";
		}
		os << " </core>\n";
	}

	os << "</schedule>\n"i;
	*/
}

SnekkjaCSchedule*
SnekkjaCSchedule::clone() const
{
	return new SnekkjaCSchedule();
}
