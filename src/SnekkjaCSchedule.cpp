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
	os << "\n\
#include <snekkja/schedule.h> \n\
\n\
#define TASK_NAME merge\n\
#define TASK_ID 1\n\
#include <snekkja/node.h>\n\
#define DONE_merge1 1\n\
\n\
size_t _snekkja_p;\n\
size_t _snekkja_n;\n\
size_t *_snekkja_tasks_in_core;\n\
size_t *_snekkja_consumers_in_core;\n\
size_t *_snekkja_producers_in_core;\n\
snekkja_schedule_task_t **_snekkja_schedule;\n\
size_t *_snekkja_tasks_in_core;\n\
size_t *_snekkja_consumers_in_core;\n\
size_t *_snekkja_producers_in_core;\n\
size_t *_snekkja_consumers_in_task;\n\
size_t *_snekkja_producers_in_task;\n\
size_t *_snekkja_remote_consumers_in_task;\n\
size_t *_snekkja_remote_producers_in_task;\n\
size_t **_snekkja_consumers_id;\n\
size_t **_snekkja_producers_id;\n\
\n\
void snekkja_schedule_init()\n\
{\n\
	_snekkja_p = 6;\n\
	_snekkja_n = 128;\n\
\n\
	_snekkja_tasks_in_core = malloc(sizeof(size_t) * _snekkja_p);\n\
	_snekkja_tasks_in_core[0] = 1;\n\
	_snekkja_tasks_in_core[1] = 2;\n\
	_snekkja_tasks_in_core[2] = 4;\n\
	_snekkja_tasks_in_core[3] = 8;\n\
	_snekkja_tasks_in_core[4] = 16;\n\
	_snekkja_tasks_in_core[5] = 32;\n\
\n\
	_snekkja_consumers_in_core = malloc(sizeof(size_t) * _snekkja_p);\n\
	_snekkja_consumers_in_core[0] = 0;\n\
	_snekkja_consumers_in_core[1] = 2;\n\
	_snekkja_consumers_in_core[2] = 4;\n\
	_snekkja_consumers_in_core[3] = 8;\n\
	_snekkja_consumers_in_core[4] = 16;\n\
	_snekkja_consumers_in_core[5] = 32;\n\
\n\
	_snekkja_producers_in_core = malloc(sizeof(size_t) * _snekkja_p);\n\
	_snekkja_producers_in_core[0] = 2;\n\
	_snekkja_producers_in_core[1] = 4;\n\
	_snekkja_producers_in_core[2] = 8;\n\
	_snekkja_producers_in_core[3] = 16;\n\
	_snekkja_producers_in_core[4] = 32;\n\
	_snekkja_producers_in_core[5] = 0;\n\
\n\
	_snekkja_consumers_in_task = malloc(sizeof(size_t) * _snekkja_n);\n\
	_snekkja_consumers_in_task[0] = 0;\n\
	_snekkja_consumers_in_task[1] = 0;\n\
	_snekkja_consumers_in_task[2] = 0;\n\
	_snekkja_consumers_in_task[3] = 0;\n\
	_snekkja_consumers_in_task[4] = 1;\n\
	_snekkja_consumers_in_task[5] = 1;\n\
	_snekkja_consumers_in_task[6] = 0;\n\
	_snekkja_consumers_in_task[7] = 0;\n\
	_snekkja_consumers_in_task[8] = 1;\n\
	_snekkja_consumers_in_task[9] = 1;\n\
	_snekkja_consumers_in_task[10] = 1;\n\
	_snekkja_consumers_in_task[11] = 1;\n\
	_snekkja_consumers_in_task[12] = 0;\n\
	_snekkja_consumers_in_task[13] = 0;\n\
	_snekkja_consumers_in_task[14] = 0;\n\
	_snekkja_consumers_in_task[15] = 0;\n\
	_snekkja_consumers_in_task[16] = 1;\n\
	_snekkja_consumers_in_task[17] = 1;\n\
	_snekkja_consumers_in_task[18] = 1;\n\
	_snekkja_consumers_in_task[19] = 1;\n\
	_snekkja_consumers_in_task[20] = 1;\n\
	_snekkja_consumers_in_task[21] = 1;\n\
	_snekkja_consumers_in_task[22] = 1;\n\
	_snekkja_consumers_in_task[23] = 1;\n\
	_snekkja_consumers_in_task[24] = 0;\n\
	_snekkja_consumers_in_task[25] = 0;\n\
	_snekkja_consumers_in_task[26] = 0;\n\
	_snekkja_consumers_in_task[27] = 0;\n\
	_snekkja_consumers_in_task[28] = 0;\n\
	_snekkja_consumers_in_task[29] = 0;\n\
	_snekkja_consumers_in_task[30] = 0;\n\
	_snekkja_consumers_in_task[31] = 0;\n\
	_snekkja_consumers_in_task[32] = 1;\n\
	_snekkja_consumers_in_task[33] = 1;\n\
	_snekkja_consumers_in_task[34] = 1;\n\
	_snekkja_consumers_in_task[35] = 1;\n\
	_snekkja_consumers_in_task[36] = 1;\n\
	_snekkja_consumers_in_task[37] = 1;\n\
	_snekkja_consumers_in_task[38] = 1;\n\
	_snekkja_consumers_in_task[39] = 1;\n\
	_snekkja_consumers_in_task[40] = 1;\n\
	_snekkja_consumers_in_task[41] = 1;\n\
	_snekkja_consumers_in_task[42] = 1;\n\
	_snekkja_consumers_in_task[43] = 1;\n\
	_snekkja_consumers_in_task[44] = 1;\n\
	_snekkja_consumers_in_task[45] = 1;\n\
	_snekkja_consumers_in_task[46] = 1;\n\
	_snekkja_consumers_in_task[47] = 1;\n\
	_snekkja_consumers_in_task[48] = 0;\n\
	_snekkja_consumers_in_task[49] = 0;\n\
	_snekkja_consumers_in_task[50] = 0;\n\
	_snekkja_consumers_in_task[51] = 0;\n\
	_snekkja_consumers_in_task[52] = 0;\n\
	_snekkja_consumers_in_task[53] = 0;\n\
	_snekkja_consumers_in_task[54] = 0;\n\
	_snekkja_consumers_in_task[55] = 0;\n\
	_snekkja_consumers_in_task[56] = 0;\n\
	_snekkja_consumers_in_task[67] = 0;\n\
	_snekkja_consumers_in_task[58] = 0;\n\
	_snekkja_consumers_in_task[59] = 0;\n\
	_snekkja_consumers_in_task[60] = 0;\n\
	_snekkja_consumers_in_task[61] = 0;\n\
	_snekkja_consumers_in_task[62] = 0;\n\
	_snekkja_consumers_in_task[63] = 0;\n\
	_snekkja_consumers_in_task[64] = 1;\n\
	_snekkja_consumers_in_task[65] = 1;\n\
	_snekkja_consumers_in_task[66] = 1;\n\
	_snekkja_consumers_in_task[67] = 1;\n\
	_snekkja_consumers_in_task[68] = 1;\n\
	_snekkja_consumers_in_task[69] = 1;\n\
	_snekkja_consumers_in_task[70] = 1;\n\
	_snekkja_consumers_in_task[71] = 1;\n\
	_snekkja_consumers_in_task[72] = 1;\n\
	_snekkja_consumers_in_task[73] = 1;\n\
	_snekkja_consumers_in_task[74] = 1;\n\
	_snekkja_consumers_in_task[75] = 1;\n\
	_snekkja_consumers_in_task[76] = 1;\n\
	_snekkja_consumers_in_task[77] = 1;\n\
	_snekkja_consumers_in_task[78] = 1;\n\
	_snekkja_consumers_in_task[79] = 1;\n\
	_snekkja_consumers_in_task[80] = 1;\n\
	_snekkja_consumers_in_task[81] = 1;\n\
	_snekkja_consumers_in_task[82] = 1;\n\
	_snekkja_consumers_in_task[83] = 1;\n\
	_snekkja_consumers_in_task[84] = 1;\n\
	_snekkja_consumers_in_task[85] = 1;\n\
	_snekkja_consumers_in_task[86] = 1;\n\
	_snekkja_consumers_in_task[87] = 1;\n\
	_snekkja_consumers_in_task[88] = 1;\n\
	_snekkja_consumers_in_task[89] = 1;\n\
	_snekkja_consumers_in_task[90] = 1;\n\
	_snekkja_consumers_in_task[91] = 1;\n\
	_snekkja_consumers_in_task[92] = 1;\n\
	_snekkja_consumers_in_task[93] = 1;\n\
	_snekkja_consumers_in_task[94] = 1;\n\
	_snekkja_consumers_in_task[95] = 1;\n\
	_snekkja_consumers_in_task[96] = 0;\n\
	_snekkja_consumers_in_task[97] = 0;\n\
	_snekkja_consumers_in_task[98] = 0;\n\
	_snekkja_consumers_in_task[99] = 0;\n\
	_snekkja_consumers_in_task[100] = 0;\n\
	_snekkja_consumers_in_task[101] = 0;\n\
	_snekkja_consumers_in_task[102] = 0;\n\
	_snekkja_consumers_in_task[103] = 0;\n\
	_snekkja_consumers_in_task[104] = 0;\n\
	_snekkja_consumers_in_task[105] = 0;\n\
	_snekkja_consumers_in_task[106] = 0;\n\
	_snekkja_consumers_in_task[107] = 0;\n\
	_snekkja_consumers_in_task[108] = 0;\n\
	_snekkja_consumers_in_task[109] = 0;\n\
	_snekkja_consumers_in_task[110] = 0;\n\
	_snekkja_consumers_in_task[111] = 0;\n\
	_snekkja_consumers_in_task[112] = 0;\n\
	_snekkja_consumers_in_task[113] = 0;\n\
	_snekkja_consumers_in_task[114] = 0;\n\
	_snekkja_consumers_in_task[115] = 0;\n\
	_snekkja_consumers_in_task[116] = 0;\n\
	_snekkja_consumers_in_task[117] = 0;\n\
	_snekkja_consumers_in_task[118] = 0;\n\
	_snekkja_consumers_in_task[119] = 0;\n\
	_snekkja_consumers_in_task[120] = 0;\n\
	_snekkja_consumers_in_task[121] = 0;\n\
	_snekkja_consumers_in_task[122] = 0;\n\
	_snekkja_consumers_in_task[123] = 0;\n\
	_snekkja_consumers_in_task[124] = 0;\n\
	_snekkja_consumers_in_task[125] = 0;\n\
	_snekkja_consumers_in_task[126] = 0;\n\
	_snekkja_consumers_in_task[127] = 0;\n\
\n\
	_snekkja_producers_in_task = malloc(sizeof(size_t) * _snekkja_n);\n\
	_snekkja_producers_in_task[0] = 0;\n\
	_snekkja_producers_in_task[1] = 0;\n\
	_snekkja_producers_in_task[2] = 2;\n\
	_snekkja_producers_in_task[3] = 0;\n\
	_snekkja_producers_in_task[4] = 2;\n\
	_snekkja_producers_in_task[5] = 2;\n\
	_snekkja_producers_in_task[6] = 0;\n\
	_snekkja_producers_in_task[7] = 0;\n\
	_snekkja_producers_in_task[8] = 2;\n\
	_snekkja_producers_in_task[9] = 2;\n\
	_snekkja_producers_in_task[10] = 2;\n\
	_snekkja_producers_in_task[11] = 2;\n\
	_snekkja_producers_in_task[12] = 0;\n\
	_snekkja_producers_in_task[13] = 0;\n\
	_snekkja_producers_in_task[14] = 0;\n\
	_snekkja_producers_in_task[15] = 0;\n\
	_snekkja_producers_in_task[16] = 2;\n\
	_snekkja_producers_in_task[17] = 2;\n\
	_snekkja_producers_in_task[18] = 2;\n\
	_snekkja_producers_in_task[19] = 2;\n\
	_snekkja_producers_in_task[20] = 2;\n\
	_snekkja_producers_in_task[21] = 2;\n\
	_snekkja_producers_in_task[22] = 2;\n\
	_snekkja_producers_in_task[23] = 2;\n\
	_snekkja_producers_in_task[24] = 0;\n\
	_snekkja_producers_in_task[25] = 0;\n\
	_snekkja_producers_in_task[26] = 0;\n\
	_snekkja_producers_in_task[27] = 0;\n\
	_snekkja_producers_in_task[28] = 0;\n\
	_snekkja_producers_in_task[29] = 0;\n\
	_snekkja_producers_in_task[30] = 0;\n\
	_snekkja_producers_in_task[31] = 0;\n\
	_snekkja_producers_in_task[32] = 2;\n\
	_snekkja_producers_in_task[33] = 2;\n\
	_snekkja_producers_in_task[34] = 2;\n\
	_snekkja_producers_in_task[35] = 2;\n\
	_snekkja_producers_in_task[36] = 2;\n\
	_snekkja_producers_in_task[37] = 2;\n\
	_snekkja_producers_in_task[38] = 2;\n\
	_snekkja_producers_in_task[39] = 2;\n\
	_snekkja_producers_in_task[40] = 2;\n\
	_snekkja_producers_in_task[41] = 2;\n\
	_snekkja_producers_in_task[42] = 2;\n\
	_snekkja_producers_in_task[43] = 2;\n\
	_snekkja_producers_in_task[44] = 2;\n\
	_snekkja_producers_in_task[45] = 2;\n\
	_snekkja_producers_in_task[46] = 2;\n\
	_snekkja_producers_in_task[47] = 2;\n\
	_snekkja_producers_in_task[48] = 0;\n\
	_snekkja_producers_in_task[49] = 0;\n\
	_snekkja_producers_in_task[50] = 0;\n\
	_snekkja_producers_in_task[51] = 0;\n\
	_snekkja_producers_in_task[52] = 0;\n\
	_snekkja_producers_in_task[53] = 0;\n\
	_snekkja_producers_in_task[54] = 0;\n\
	_snekkja_producers_in_task[55] = 0;\n\
	_snekkja_producers_in_task[56] = 0;\n\
	_snekkja_producers_in_task[57] = 0;\n\
	_snekkja_producers_in_task[58] = 0;\n\
	_snekkja_producers_in_task[59] = 0;\n\
	_snekkja_producers_in_task[60] = 0;\n\
	_snekkja_producers_in_task[61] = 0;\n\
	_snekkja_producers_in_task[62] = 0;\n\
	_snekkja_producers_in_task[63] = 0;\n\
	_snekkja_producers_in_task[64] = 0;\n\
	_snekkja_producers_in_task[65] = 0;\n\
	_snekkja_producers_in_task[66] = 0;\n\
	_snekkja_producers_in_task[67] = 0;\n\
	_snekkja_producers_in_task[68] = 0;\n\
	_snekkja_producers_in_task[69] = 0;\n\
	_snekkja_producers_in_task[70] = 0;\n\
	_snekkja_producers_in_task[71] = 0;\n\
	_snekkja_producers_in_task[72] = 0;\n\
	_snekkja_producers_in_task[73] = 0;\n\
	_snekkja_producers_in_task[74] = 0;\n\
	_snekkja_producers_in_task[75] = 0;\n\
	_snekkja_producers_in_task[76] = 0;\n\
	_snekkja_producers_in_task[77] = 0;\n\
	_snekkja_producers_in_task[78] = 0;\n\
	_snekkja_producers_in_task[79] = 0;\n\
	_snekkja_producers_in_task[80] = 0;\n\
	_snekkja_producers_in_task[81] = 0;\n\
	_snekkja_producers_in_task[82] = 0;\n\
	_snekkja_producers_in_task[83] = 0;\n\
	_snekkja_producers_in_task[84] = 0;\n\
	_snekkja_producers_in_task[85] = 0;\n\
	_snekkja_producers_in_task[86] = 0;\n\
	_snekkja_producers_in_task[87] = 0;\n\
	_snekkja_producers_in_task[88] = 0;\n\
	_snekkja_producers_in_task[89] = 0;\n\
	_snekkja_producers_in_task[90] = 0;\n\
	_snekkja_producers_in_task[91] = 0;\n\
	_snekkja_producers_in_task[92] = 0;\n\
	_snekkja_producers_in_task[93] = 0;\n\
	_snekkja_producers_in_task[94] = 0;\n\
	_snekkja_producers_in_task[95] = 0;\n\
	_snekkja_producers_in_task[96] = 0;\n\
	_snekkja_producers_in_task[97] = 0;\n\
	_snekkja_producers_in_task[98] = 0;\n\
	_snekkja_producers_in_task[99] = 0;\n\
	_snekkja_producers_in_task[100] = 0;\n\
	_snekkja_producers_in_task[101] = 0;\n\
	_snekkja_producers_in_task[102] = 0;\n\
	_snekkja_producers_in_task[103] = 0;\n\
	_snekkja_producers_in_task[104] = 0;\n\
	_snekkja_producers_in_task[105] = 0;\n\
	_snekkja_producers_in_task[106] = 0;\n\
	_snekkja_producers_in_task[107] = 0;\n\
	_snekkja_producers_in_task[108] = 0;\n\
	_snekkja_producers_in_task[109] = 0;\n\
	_snekkja_producers_in_task[110] = 0;\n\
	_snekkja_producers_in_task[111] = 0;\n\
	_snekkja_producers_in_task[112] = 0;\n\
	_snekkja_producers_in_task[113] = 0;\n\
	_snekkja_producers_in_task[114] = 0;\n\
	_snekkja_producers_in_task[115] = 0;\n\
	_snekkja_producers_in_task[116] = 0;\n\
	_snekkja_producers_in_task[117] = 0;\n\
	_snekkja_producers_in_task[118] = 0;\n\
	_snekkja_producers_in_task[119] = 0;\n\
	_snekkja_producers_in_task[120] = 0;\n\
	_snekkja_producers_in_task[121] = 0;\n\
	_snekkja_producers_in_task[122] = 0;\n\
	_snekkja_producers_in_task[123] = 0;\n\
	_snekkja_producers_in_task[124] = 0;\n\
	_snekkja_producers_in_task[125] = 0;\n\
	_snekkja_producers_in_task[126] = 0;\n\
	_snekkja_producers_in_task[127] = 0;\n\
\n\
	_snekkja_remote_consumers_in_task = malloc(sizeof(size_t) * _snekkja_n);\n\
	_snekkja_remote_consumers_in_task[0] = 0;\n\
	_snekkja_remote_consumers_in_task[1] = 0;\n\
	_snekkja_remote_consumers_in_task[2] = 0;\n\
	_snekkja_remote_consumers_in_task[3] = 0;\n\
	_snekkja_remote_consumers_in_task[4] = 1;\n\
	_snekkja_remote_consumers_in_task[5] = 1;\n\
	_snekkja_remote_consumers_in_task[6] = 0;\n\
	_snekkja_remote_consumers_in_task[7] = 0;\n\
	_snekkja_remote_consumers_in_task[8] = 1;\n\
	_snekkja_remote_consumers_in_task[9] = 1;\n\
	_snekkja_remote_consumers_in_task[10] = 1;\n\
	_snekkja_remote_consumers_in_task[11] = 1;\n\
	_snekkja_remote_consumers_in_task[12] = 0;\n\
	_snekkja_remote_consumers_in_task[13] = 0;\n\
	_snekkja_remote_consumers_in_task[14] = 0;\n\
	_snekkja_remote_consumers_in_task[15] = 0;\n\
	_snekkja_remote_consumers_in_task[16] = 1;\n\
	_snekkja_remote_consumers_in_task[17] = 1;\n\
	_snekkja_remote_consumers_in_task[18] = 1;\n\
	_snekkja_remote_consumers_in_task[19] = 1;\n\
	_snekkja_remote_consumers_in_task[20] = 1;\n\
	_snekkja_remote_consumers_in_task[21] = 1;\n\
	_snekkja_remote_consumers_in_task[22] = 1;\n\
	_snekkja_remote_consumers_in_task[23] = 1;\n\
	_snekkja_remote_consumers_in_task[24] = 0;\n\
	_snekkja_remote_consumers_in_task[25] = 0;\n\
	_snekkja_remote_consumers_in_task[26] = 0;\n\
	_snekkja_remote_consumers_in_task[27] = 0;\n\
	_snekkja_remote_consumers_in_task[28] = 0;\n\
	_snekkja_remote_consumers_in_task[29] = 0;\n\
	_snekkja_remote_consumers_in_task[30] = 0;\n\
	_snekkja_remote_consumers_in_task[31] = 0;\n\
	_snekkja_remote_consumers_in_task[32] = 1;\n\
	_snekkja_remote_consumers_in_task[33] = 1;\n\
	_snekkja_remote_consumers_in_task[34] = 1;\n\
	_snekkja_remote_consumers_in_task[35] = 1;\n\
	_snekkja_remote_consumers_in_task[36] = 1;\n\
	_snekkja_remote_consumers_in_task[37] = 1;\n\
	_snekkja_remote_consumers_in_task[38] = 1;\n\
	_snekkja_remote_consumers_in_task[39] = 1;\n\
	_snekkja_remote_consumers_in_task[40] = 1;\n\
	_snekkja_remote_consumers_in_task[41] = 1;\n\
	_snekkja_remote_consumers_in_task[42] = 1;\n\
	_snekkja_remote_consumers_in_task[43] = 1;\n\
	_snekkja_remote_consumers_in_task[44] = 1;\n\
	_snekkja_remote_consumers_in_task[45] = 1;\n\
	_snekkja_remote_consumers_in_task[46] = 1;\n\
	_snekkja_remote_consumers_in_task[47] = 1;\n\
	_snekkja_remote_consumers_in_task[48] = 0;\n\
	_snekkja_remote_consumers_in_task[49] = 0;\n\
	_snekkja_remote_consumers_in_task[50] = 0;\n\
	_snekkja_remote_consumers_in_task[51] = 0;\n\
	_snekkja_remote_consumers_in_task[52] = 0;\n\
	_snekkja_remote_consumers_in_task[53] = 0;\n\
	_snekkja_remote_consumers_in_task[54] = 0;\n\
	_snekkja_remote_consumers_in_task[55] = 0;\n\
	_snekkja_remote_consumers_in_task[56] = 0;\n\
	_snekkja_remote_consumers_in_task[67] = 0;\n\
	_snekkja_remote_consumers_in_task[58] = 0;\n\
	_snekkja_remote_consumers_in_task[59] = 0;\n\
	_snekkja_remote_consumers_in_task[60] = 0;\n\
	_snekkja_remote_consumers_in_task[61] = 0;\n\
	_snekkja_remote_consumers_in_task[62] = 0;\n\
	_snekkja_remote_consumers_in_task[63] = 0;\n\
	_snekkja_remote_consumers_in_task[64] = 1;\n\
	_snekkja_remote_consumers_in_task[65] = 1;\n\
	_snekkja_remote_consumers_in_task[66] = 1;\n\
	_snekkja_remote_consumers_in_task[67] = 1;\n\
	_snekkja_remote_consumers_in_task[68] = 1;\n\
	_snekkja_remote_consumers_in_task[69] = 1;\n\
	_snekkja_remote_consumers_in_task[70] = 1;\n\
	_snekkja_remote_consumers_in_task[71] = 1;\n\
	_snekkja_remote_consumers_in_task[72] = 1;\n\
	_snekkja_remote_consumers_in_task[73] = 1;\n\
	_snekkja_remote_consumers_in_task[74] = 1;\n\
	_snekkja_remote_consumers_in_task[75] = 1;\n\
	_snekkja_remote_consumers_in_task[76] = 1;\n\
	_snekkja_remote_consumers_in_task[77] = 1;\n\
	_snekkja_remote_consumers_in_task[78] = 1;\n\
	_snekkja_remote_consumers_in_task[79] = 1;\n\
	_snekkja_remote_consumers_in_task[80] = 1;\n\
	_snekkja_remote_consumers_in_task[81] = 1;\n\
	_snekkja_remote_consumers_in_task[82] = 1;\n\
	_snekkja_remote_consumers_in_task[83] = 1;\n\
	_snekkja_remote_consumers_in_task[84] = 1;\n\
	_snekkja_remote_consumers_in_task[85] = 1;\n\
	_snekkja_remote_consumers_in_task[86] = 1;\n\
	_snekkja_remote_consumers_in_task[87] = 1;\n\
	_snekkja_remote_consumers_in_task[88] = 1;\n\
	_snekkja_remote_consumers_in_task[89] = 1;\n\
	_snekkja_remote_consumers_in_task[90] = 1;\n\
	_snekkja_remote_consumers_in_task[91] = 1;\n\
	_snekkja_remote_consumers_in_task[92] = 1;\n\
	_snekkja_remote_consumers_in_task[93] = 1;\n\
	_snekkja_remote_consumers_in_task[94] = 1;\n\
	_snekkja_remote_consumers_in_task[95] = 1;\n\
	_snekkja_remote_consumers_in_task[96] = 0;\n\
	_snekkja_remote_consumers_in_task[97] = 0;\n\
	_snekkja_remote_consumers_in_task[98] = 0;\n\
	_snekkja_remote_consumers_in_task[99] = 0;\n\
	_snekkja_remote_consumers_in_task[100] = 0;\n\
	_snekkja_remote_consumers_in_task[101] = 0;\n\
	_snekkja_remote_consumers_in_task[102] = 0;\n\
	_snekkja_remote_consumers_in_task[103] = 0;\n\
	_snekkja_remote_consumers_in_task[104] = 0;\n\
	_snekkja_remote_consumers_in_task[105] = 0;\n\
	_snekkja_remote_consumers_in_task[106] = 0;\n\
	_snekkja_remote_consumers_in_task[107] = 0;\n\
	_snekkja_remote_consumers_in_task[108] = 0;\n\
	_snekkja_remote_consumers_in_task[109] = 0;\n\
	_snekkja_remote_consumers_in_task[110] = 0;\n\
	_snekkja_remote_consumers_in_task[111] = 0;\n\
	_snekkja_remote_consumers_in_task[112] = 0;\n\
	_snekkja_remote_consumers_in_task[113] = 0;\n\
	_snekkja_remote_consumers_in_task[114] = 0;\n\
	_snekkja_remote_consumers_in_task[115] = 0;\n\
	_snekkja_remote_consumers_in_task[116] = 0;\n\
	_snekkja_remote_consumers_in_task[117] = 0;\n\
	_snekkja_remote_consumers_in_task[118] = 0;\n\
	_snekkja_remote_consumers_in_task[119] = 0;\n\
	_snekkja_remote_consumers_in_task[120] = 0;\n\
	_snekkja_remote_consumers_in_task[121] = 0;\n\
	_snekkja_remote_consumers_in_task[122] = 0;\n\
	_snekkja_remote_consumers_in_task[123] = 0;\n\
	_snekkja_remote_consumers_in_task[124] = 0;\n\
	_snekkja_remote_consumers_in_task[125] = 0;\n\
	_snekkja_remote_consumers_in_task[126] = 0;\n\
	_snekkja_remote_consumers_in_task[127] = 0;\n\
\n\
	_snekkja_remote_producers_in_task = malloc(sizeof(size_t) * _snekkja_n);\n\
	_snekkja_remote_producers_in_task[0] = 0;\n\
	_snekkja_remote_producers_in_task[1] = 0;\n\
	_snekkja_remote_producers_in_task[2] = 2;\n\
	_snekkja_remote_producers_in_task[3] = 0;\n\
	_snekkja_remote_producers_in_task[4] = 2;\n\
	_snekkja_remote_producers_in_task[5] = 2;\n\
	_snekkja_remote_producers_in_task[6] = 0;\n\
	_snekkja_remote_producers_in_task[7] = 0;\n\
	_snekkja_remote_producers_in_task[8] = 2;\n\
	_snekkja_remote_producers_in_task[9] = 2;\n\
	_snekkja_remote_producers_in_task[10] = 2;\n\
	_snekkja_remote_producers_in_task[11] = 2;\n\
	_snekkja_remote_producers_in_task[12] = 0;\n\
	_snekkja_remote_producers_in_task[13] = 0;\n\
	_snekkja_remote_producers_in_task[14] = 0;\n\
	_snekkja_remote_producers_in_task[15] = 0;\n\
	_snekkja_remote_producers_in_task[16] = 2;\n\
	_snekkja_remote_producers_in_task[17] = 2;\n\
	_snekkja_remote_producers_in_task[18] = 2;\n\
	_snekkja_remote_producers_in_task[19] = 2;\n\
	_snekkja_remote_producers_in_task[20] = 2;\n\
	_snekkja_remote_producers_in_task[21] = 2;\n\
	_snekkja_remote_producers_in_task[22] = 2;\n\
	_snekkja_remote_producers_in_task[23] = 2;\n\
	_snekkja_remote_producers_in_task[24] = 0;\n\
	_snekkja_remote_producers_in_task[25] = 0;\n\
	_snekkja_remote_producers_in_task[26] = 0;\n\
	_snekkja_remote_producers_in_task[27] = 0;\n\
	_snekkja_remote_producers_in_task[28] = 0;\n\
	_snekkja_remote_producers_in_task[29] = 0;\n\
	_snekkja_remote_producers_in_task[30] = 0;\n\
	_snekkja_remote_producers_in_task[31] = 0;\n\
	_snekkja_remote_producers_in_task[32] = 2;\n\
	_snekkja_remote_producers_in_task[33] = 2;\n\
	_snekkja_remote_producers_in_task[34] = 2;\n\
	_snekkja_remote_producers_in_task[35] = 2;\n\
	_snekkja_remote_producers_in_task[36] = 2;\n\
	_snekkja_remote_producers_in_task[37] = 2;\n\
	_snekkja_remote_producers_in_task[38] = 2;\n\
	_snekkja_remote_producers_in_task[39] = 2;\n\
	_snekkja_remote_producers_in_task[40] = 2;\n\
	_snekkja_remote_producers_in_task[41] = 2;\n\
	_snekkja_remote_producers_in_task[42] = 2;\n\
	_snekkja_remote_producers_in_task[43] = 2;\n\
	_snekkja_remote_producers_in_task[44] = 2;\n\
	_snekkja_remote_producers_in_task[45] = 2;\n\
	_snekkja_remote_producers_in_task[46] = 2;\n\
	_snekkja_remote_producers_in_task[47] = 2;\n\
	_snekkja_remote_producers_in_task[48] = 0;\n\
	_snekkja_remote_producers_in_task[49] = 0;\n\
	_snekkja_remote_producers_in_task[50] = 0;\n\
	_snekkja_remote_producers_in_task[51] = 0;\n\
	_snekkja_remote_producers_in_task[52] = 0;\n\
	_snekkja_remote_producers_in_task[53] = 0;\n\
	_snekkja_remote_producers_in_task[54] = 0;\n\
	_snekkja_remote_producers_in_task[55] = 0;\n\
	_snekkja_remote_producers_in_task[56] = 0;\n\
	_snekkja_remote_producers_in_task[57] = 0;\n\
	_snekkja_remote_producers_in_task[58] = 0;\n\
	_snekkja_remote_producers_in_task[59] = 0;\n\
	_snekkja_remote_producers_in_task[60] = 0;\n\
	_snekkja_remote_producers_in_task[61] = 0;\n\
	_snekkja_remote_producers_in_task[62] = 0;\n\
	_snekkja_remote_producers_in_task[63] = 0;\n\
	_snekkja_remote_producers_in_task[64] = 0;\n\
	_snekkja_remote_producers_in_task[65] = 0;\n\
	_snekkja_remote_producers_in_task[66] = 0;\n\
	_snekkja_remote_producers_in_task[67] = 0;\n\
	_snekkja_remote_producers_in_task[68] = 0;\n\
	_snekkja_remote_producers_in_task[69] = 0;\n\
	_snekkja_remote_producers_in_task[70] = 0;\n\
	_snekkja_remote_producers_in_task[71] = 0;\n\
	_snekkja_remote_producers_in_task[72] = 0;\n\
	_snekkja_remote_producers_in_task[73] = 0;\n\
	_snekkja_remote_producers_in_task[74] = 0;\n\
	_snekkja_remote_producers_in_task[75] = 0;\n\
	_snekkja_remote_producers_in_task[76] = 0;\n\
	_snekkja_remote_producers_in_task[77] = 0;\n\
	_snekkja_remote_producers_in_task[78] = 0;\n\
	_snekkja_remote_producers_in_task[79] = 0;\n\
	_snekkja_remote_producers_in_task[80] = 0;\n\
	_snekkja_remote_producers_in_task[81] = 0;\n\
	_snekkja_remote_producers_in_task[82] = 0;\n\
	_snekkja_remote_producers_in_task[83] = 0;\n\
	_snekkja_remote_producers_in_task[84] = 0;\n\
	_snekkja_remote_producers_in_task[85] = 0;\n\
	_snekkja_remote_producers_in_task[86] = 0;\n\
	_snekkja_remote_producers_in_task[87] = 0;\n\
	_snekkja_remote_producers_in_task[88] = 0;\n\
	_snekkja_remote_producers_in_task[89] = 0;\n\
	_snekkja_remote_producers_in_task[90] = 0;\n\
	_snekkja_remote_producers_in_task[91] = 0;\n\
	_snekkja_remote_producers_in_task[92] = 0;\n\
	_snekkja_remote_producers_in_task[93] = 0;\n\
	_snekkja_remote_producers_in_task[94] = 0;\n\
	_snekkja_remote_producers_in_task[95] = 0;\n\
	_snekkja_remote_producers_in_task[96] = 0;\n\
	_snekkja_remote_producers_in_task[97] = 0;\n\
	_snekkja_remote_producers_in_task[98] = 0;\n\
	_snekkja_remote_producers_in_task[99] = 0;\n\
	_snekkja_remote_producers_in_task[100] = 0;\n\
	_snekkja_remote_producers_in_task[101] = 0;\n\
	_snekkja_remote_producers_in_task[102] = 0;\n\
	_snekkja_remote_producers_in_task[103] = 0;\n\
	_snekkja_remote_producers_in_task[104] = 0;\n\
	_snekkja_remote_producers_in_task[105] = 0;\n\
	_snekkja_remote_producers_in_task[106] = 0;\n\
	_snekkja_remote_producers_in_task[107] = 0;\n\
	_snekkja_remote_producers_in_task[108] = 0;\n\
	_snekkja_remote_producers_in_task[109] = 0;\n\
	_snekkja_remote_producers_in_task[110] = 0;\n\
	_snekkja_remote_producers_in_task[111] = 0;\n\
	_snekkja_remote_producers_in_task[112] = 0;\n\
	_snekkja_remote_producers_in_task[113] = 0;\n\
	_snekkja_remote_producers_in_task[114] = 0;\n\
	_snekkja_remote_producers_in_task[115] = 0;\n\
	_snekkja_remote_producers_in_task[116] = 0;\n\
	_snekkja_remote_producers_in_task[117] = 0;\n\
	_snekkja_remote_producers_in_task[118] = 0;\n\
	_snekkja_remote_producers_in_task[119] = 0;\n\
	_snekkja_remote_producers_in_task[120] = 0;\n\
	_snekkja_remote_producers_in_task[121] = 0;\n\
	_snekkja_remote_producers_in_task[122] = 0;\n\
	_snekkja_remote_producers_in_task[123] = 0;\n\
	_snekkja_remote_producers_in_task[124] = 0;\n\
	_snekkja_remote_producers_in_task[125] = 0;\n\
	_snekkja_remote_producers_in_task[126] = 0;\n\
	_snekkja_remote_producers_in_task[127] = 0;\n\
\n\
	_snekkja_producers_id = malloc(sizeof(size_t*) * _snekkja_n);\n\
	_snekkja_producers_id[0] = NULL;\n\
	_snekkja_producers_id[1] = NULL;\n\
	_snekkja_producers_id[2] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[2][0] = 5;\n\
	_snekkja_producers_id[2][1] = 6;\n\
	_snekkja_producers_id[3] = NULL;\n\
	_snekkja_producers_id[4] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[4][0] = 9;\n\
	_snekkja_producers_id[4][1] = 10;\n\
	_snekkja_producers_id[5] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[5][0] = 11;\n\
	_snekkja_producers_id[5][1] = 12;\n\
	_snekkja_producers_id[6] = NULL;\n\
	_snekkja_producers_id[7] = NULL;\n\
	_snekkja_producers_id[8] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[8][0] = 17;\n\
	_snekkja_producers_id[8][1] = 18;\n\
	_snekkja_producers_id[9] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[9][0] = 19;\n\
	_snekkja_producers_id[9][1] = 20;\n\
	_snekkja_producers_id[10] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[10][0] = 21;\n\
	_snekkja_producers_id[10][1] = 22;\n\
	_snekkja_producers_id[11] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[11][0] = 23;\n\
	_snekkja_producers_id[11][1] = 24;\n\
	_snekkja_producers_id[12] = NULL;\n\
	_snekkja_producers_id[13] = NULL;\n\
	_snekkja_producers_id[14] = NULL;\n\
	_snekkja_producers_id[15] = NULL;\n\
	_snekkja_producers_id[16] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[16][0] = 33;\n\
	_snekkja_producers_id[16][1] = 34;\n\
	_snekkja_producers_id[17] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[17][0] = 35;\n\
	_snekkja_producers_id[17][1] = 36;\n\
	_snekkja_producers_id[18] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[18][0] = 37;\n\
	_snekkja_producers_id[18][1] = 38;\n\
	_snekkja_producers_id[19] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[19][0] = 39;\n\
	_snekkja_producers_id[19][1] = 40;\n\
	_snekkja_producers_id[20] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[20][0] = 41;\n\
	_snekkja_producers_id[20][1] = 42;\n\
	_snekkja_producers_id[21] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[21][0] = 43;\n\
	_snekkja_producers_id[21][1] = 44;\n\
	_snekkja_producers_id[22] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[22][0] = 45;\n\
	_snekkja_producers_id[22][1] = 46;\n\
	_snekkja_producers_id[23] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[23][0] = 47;\n\
	_snekkja_producers_id[23][1] = 48;\n\
	_snekkja_producers_id[24] = NULL;\n\
	_snekkja_producers_id[25] = NULL;\n\
	_snekkja_producers_id[26] = NULL;\n\
	_snekkja_producers_id[27] = NULL;\n\
	_snekkja_producers_id[28] = NULL;\n\
	_snekkja_producers_id[29] = NULL;\n\
	_snekkja_producers_id[30] = NULL;\n\
	_snekkja_producers_id[31] = NULL;\n\
	_snekkja_producers_id[32] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[32][0] = 65;\n\
	_snekkja_producers_id[32][1] = 66;\n\
	_snekkja_producers_id[33] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[33][0] = 67;\n\
	_snekkja_producers_id[33][1] = 68;\n\
	_snekkja_producers_id[34] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[34][0] = 69;\n\
	_snekkja_producers_id[34][1] = 70;\n\
	_snekkja_producers_id[35] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[35][0] = 71;\n\
	_snekkja_producers_id[35][1] = 72;\n\
	_snekkja_producers_id[36] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[36][0] = 73;\n\
	_snekkja_producers_id[36][1] = 74;\n\
	_snekkja_producers_id[37] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[37][0] = 75;\n\
	_snekkja_producers_id[37][1] = 76;\n\
	_snekkja_producers_id[38] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[38][0] = 77;\n\
	_snekkja_producers_id[38][1] = 78;\n\
	_snekkja_producers_id[39] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[39][0] = 79;\n\
	_snekkja_producers_id[39][1] = 80;\n\
	_snekkja_producers_id[40] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[40][0] = 81;\n\
	_snekkja_producers_id[40][1] = 82;\n\
	_snekkja_producers_id[41] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[41][0] = 83;\n\
	_snekkja_producers_id[41][1] = 84;\n\
	_snekkja_producers_id[42] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[42][0] = 85;\n\
	_snekkja_producers_id[42][1] = 86;\n\
	_snekkja_producers_id[43] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[43][0] = 87;\n\
	_snekkja_producers_id[43][1] = 88;\n\
	_snekkja_producers_id[44] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[44][0] = 89;\n\
	_snekkja_producers_id[44][1] = 90;\n\
	_snekkja_producers_id[45] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[45][0] = 91;\n\
	_snekkja_producers_id[45][1] = 92;\n\
	_snekkja_producers_id[46] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[46][0] = 93;\n\
	_snekkja_producers_id[46][1] = 94;\n\
	_snekkja_producers_id[47] = malloc(sizeof(size_t) * 2);\n\
	_snekkja_producers_id[47][0] = 95;\n\
	_snekkja_producers_id[47][1] = 96;\n\
	_snekkja_producers_id[48] = NULL;\n\
	_snekkja_producers_id[49] = NULL;\n\
	_snekkja_producers_id[50] = NULL;\n\
	_snekkja_producers_id[51] = NULL;\n\
	_snekkja_producers_id[52] = NULL;\n\
	_snekkja_producers_id[53] = NULL;\n\
	_snekkja_producers_id[54] = NULL;\n\
	_snekkja_producers_id[55] = NULL;\n\
	_snekkja_producers_id[56] = NULL;\n\
	_snekkja_producers_id[57] = NULL;\n\
	_snekkja_producers_id[58] = NULL;\n\
	_snekkja_producers_id[59] = NULL;\n\
	_snekkja_producers_id[60] = NULL;\n\
	_snekkja_producers_id[61] = NULL;\n\
	_snekkja_producers_id[62] = NULL;\n\
	_snekkja_producers_id[63] = NULL;\n\
	_snekkja_producers_id[64] = NULL;\n\
	_snekkja_producers_id[65] = NULL;\n\
	_snekkja_producers_id[66] = NULL;\n\
	_snekkja_producers_id[67] = NULL;\n\
	_snekkja_producers_id[68] = NULL;\n\
	_snekkja_producers_id[69] = NULL;\n\
	_snekkja_producers_id[70] = NULL;\n\
	_snekkja_producers_id[71] = NULL;\n\
	_snekkja_producers_id[72] = NULL;\n\
	_snekkja_producers_id[73] = NULL;\n\
	_snekkja_producers_id[74] = NULL;\n\
	_snekkja_producers_id[75] = NULL;\n\
	_snekkja_producers_id[76] = NULL;\n\
	_snekkja_producers_id[77] = NULL;\n\
	_snekkja_producers_id[78] = NULL;\n\
	_snekkja_producers_id[79] = NULL;\n\
	_snekkja_producers_id[80] = NULL;\n\
	_snekkja_producers_id[81] = NULL;\n\
	_snekkja_producers_id[82] = NULL;\n\
	_snekkja_producers_id[83] = NULL;\n\
	_snekkja_producers_id[84] = NULL;\n\
	_snekkja_producers_id[85] = NULL;\n\
	_snekkja_producers_id[86] = NULL;\n\
	_snekkja_producers_id[87] = NULL;\n\
	_snekkja_producers_id[88] = NULL;\n\
	_snekkja_producers_id[89] = NULL;\n\
	_snekkja_producers_id[90] = NULL;\n\
	_snekkja_producers_id[91] = NULL;\n\
	_snekkja_producers_id[92] = NULL;\n\
	_snekkja_producers_id[93] = NULL;\n\
	_snekkja_producers_id[94] = NULL;\n\
	_snekkja_producers_id[95] = NULL;\n\
	_snekkja_producers_id[96] = NULL;\n\
	_snekkja_producers_id[97] = NULL;\n\
	_snekkja_producers_id[98] = NULL;\n\
	_snekkja_producers_id[99] = NULL;\n\
	_snekkja_producers_id[100] = NULL;\n\
	_snekkja_producers_id[101] = NULL;\n\
	_snekkja_producers_id[102] = NULL;\n\
	_snekkja_producers_id[103] = NULL;\n\
	_snekkja_producers_id[104] = NULL;\n\
	_snekkja_producers_id[105] = NULL;\n\
	_snekkja_producers_id[106] = NULL;\n\
	_snekkja_producers_id[107] = NULL;\n\
	_snekkja_producers_id[108] = NULL;\n\
	_snekkja_producers_id[109] = NULL;\n\
	_snekkja_producers_id[110] = NULL;\n\
	_snekkja_producers_id[111] = NULL;\n\
	_snekkja_producers_id[112] = NULL;\n\
	_snekkja_producers_id[113] = NULL;\n\
	_snekkja_producers_id[114] = NULL;\n\
	_snekkja_producers_id[115] = NULL;\n\
	_snekkja_producers_id[116] = NULL;\n\
	_snekkja_producers_id[117] = NULL;\n\
	_snekkja_producers_id[118] = NULL;\n\
	_snekkja_producers_id[119] = NULL;\n\
	_snekkja_producers_id[120] = NULL;\n\
	_snekkja_producers_id[121] = NULL;\n\
	_snekkja_producers_id[122] = NULL;\n\
	_snekkja_producers_id[123] = NULL;\n\
	_snekkja_producers_id[124] = NULL;\n\
	_snekkja_producers_id[125] = NULL;\n\
	_snekkja_producers_id[126] = NULL;\n\
	_snekkja_producers_id[127] = NULL;\n\
\n\
	_snekkja_consumers_id = malloc(sizeof(size_t*) * _snekkja_n);\n\
	_snekkja_consumers_id[0] = NULL;\n\
	_snekkja_consumers_id[1] = NULL;\n\
	_snekkja_consumers_id[2] = NULL;\n\
	_snekkja_consumers_id[3] = NULL;\n\
	_snekkja_consumers_id[4] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[4][0] = 3;\n\
	_snekkja_consumers_id[5] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[5][0] = 3;\n\
	_snekkja_consumers_id[6] = NULL;\n\
	_snekkja_consumers_id[7] = NULL;\n\
	_snekkja_consumers_id[8] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[8][0] = 4;\n\
	_snekkja_consumers_id[9] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[9][0] = 4;\n\
	_snekkja_consumers_id[10] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[10][0] = 5;\n\
	_snekkja_consumers_id[11] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[11][0] = 5;\n\
	_snekkja_consumers_id[12] = NULL;\n\
	_snekkja_consumers_id[13] = NULL;\n\
	_snekkja_consumers_id[14] = NULL;\n\
	_snekkja_consumers_id[15] = NULL;\n\
	_snekkja_consumers_id[16] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[16][0] = 8;\n\
	_snekkja_consumers_id[17] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[17][0] = 8;\n\
	_snekkja_consumers_id[18] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[18][0] = 9;\n\
	_snekkja_consumers_id[19] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[19][0] = 9;\n\
	_snekkja_consumers_id[20] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[20][0] = 10;\n\
	_snekkja_consumers_id[21] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[21][0] = 10;\n\
	_snekkja_consumers_id[22] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[22][0] = 11;\n\
	_snekkja_consumers_id[23] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[23][0] = 11;\n\
	_snekkja_consumers_id[24] = NULL;\n\
	_snekkja_consumers_id[25] = NULL;\n\
	_snekkja_consumers_id[26] = NULL;\n\
	_snekkja_consumers_id[27] = NULL;\n\
	_snekkja_consumers_id[28] = NULL;\n\
	_snekkja_consumers_id[29] = NULL;\n\
	_snekkja_consumers_id[30] = NULL;\n\
	_snekkja_consumers_id[31] = NULL;\n\
	_snekkja_consumers_id[32] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[32][0] = 16;\n\
	_snekkja_consumers_id[33] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[33][0] = 16;\n\
	_snekkja_consumers_id[34] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[34][0] = 17;\n\
	_snekkja_consumers_id[35] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[35][0] = 17;\n\
	_snekkja_consumers_id[36] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[36][0] = 18;\n\
	_snekkja_consumers_id[37] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[37][0] = 18;\n\
	_snekkja_consumers_id[38] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[38][0] = 19;\n\
	_snekkja_consumers_id[39] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[39][0] = 19;\n\
	_snekkja_consumers_id[40] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[40][0] = 20;\n\
	_snekkja_consumers_id[41] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[41][0] = 20;\n\
	_snekkja_consumers_id[42] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[42][0] = 21;\n\
	_snekkja_consumers_id[43] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[43][0] = 21;\n\
	_snekkja_consumers_id[44] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[44][0] = 22;\n\
	_snekkja_consumers_id[45] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[45][0] = 22;\n\
	_snekkja_consumers_id[46] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[46][0] = 23;\n\
	_snekkja_consumers_id[47] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[47][0] = 23;\n\
	_snekkja_consumers_id[48] = NULL;\n\
	_snekkja_consumers_id[49] = NULL;\n\
	_snekkja_consumers_id[50] = NULL;\n\
	_snekkja_consumers_id[51] = NULL;\n\
	_snekkja_consumers_id[52] = NULL;\n\
	_snekkja_consumers_id[53] = NULL;\n\
	_snekkja_consumers_id[54] = NULL;\n\
	_snekkja_consumers_id[55] = NULL;\n\
	_snekkja_consumers_id[56] = NULL;\n\
	_snekkja_consumers_id[57] = NULL;\n\
	_snekkja_consumers_id[58] = NULL;\n\
	_snekkja_consumers_id[59] = NULL;\n\
	_snekkja_consumers_id[60] = NULL;\n\
	_snekkja_consumers_id[61] = NULL;\n\
	_snekkja_consumers_id[62] = NULL;\n\
	_snekkja_consumers_id[63] = NULL;\n\
	_snekkja_consumers_id[64] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[64][0] = 32;\n\
	_snekkja_consumers_id[65] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[65][0] = 32;\n\
	_snekkja_consumers_id[66] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[66][0] = 33;\n\
	_snekkja_consumers_id[67] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[67][0] = 33;\n\
	_snekkja_consumers_id[68] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[68][0] = 34;\n\
	_snekkja_consumers_id[69] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[69][0] = 34;\n\
	_snekkja_consumers_id[70] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[70][0] = 35;\n\
	_snekkja_consumers_id[71] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[71][0] = 35;\n\
	_snekkja_consumers_id[72] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[72][0] = 36;\n\
	_snekkja_consumers_id[73] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[73][0] = 36;\n\
	_snekkja_consumers_id[74] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[74][0] = 37;\n\
	_snekkja_consumers_id[75] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[75][0] = 37;\n\
	_snekkja_consumers_id[76] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[76][0] = 38;\n\
	_snekkja_consumers_id[77] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[77][0] = 38;\n\
	_snekkja_consumers_id[78] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[78][0] = 39;\n\
	_snekkja_consumers_id[79] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[79][0] = 39;\n\
	_snekkja_consumers_id[80] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[80][0] = 40;\n\
	_snekkja_consumers_id[81] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[81][0] = 40;\n\
	_snekkja_consumers_id[82] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[82][0] = 41;\n\
	_snekkja_consumers_id[83] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[83][0] = 41;\n\
	_snekkja_consumers_id[84] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[84][0] = 42;\n\
	_snekkja_consumers_id[85] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[85][0] = 42;\n\
	_snekkja_consumers_id[86] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[86][0] = 43;\n\
	_snekkja_consumers_id[87] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[87][0] = 43;\n\
	_snekkja_consumers_id[88] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[88][0] = 44;\n\
	_snekkja_consumers_id[89] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[89][0] = 44;\n\
	_snekkja_consumers_id[90] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[90][0] = 45;\n\
	_snekkja_consumers_id[91] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[91][0] = 45;\n\
	_snekkja_consumers_id[92] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[92][0] = 46;\n\
	_snekkja_consumers_id[93] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[93][0] = 46;\n\
	_snekkja_consumers_id[94] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[94][0] = 47;\n\
	_snekkja_consumers_id[95] = malloc(sizeof(size_t) * 1);\n\
	_snekkja_consumers_id[95][0] = 47;\n\
	_snekkja_consumers_id[96] = NULL;\n\
	_snekkja_consumers_id[97] = NULL;\n\
	_snekkja_consumers_id[98] = NULL;\n\
	_snekkja_consumers_id[99] = NULL;\n\
	_snekkja_consumers_id[100] = NULL;\n\
	_snekkja_consumers_id[101] = NULL;\n\
	_snekkja_consumers_id[102] = NULL;\n\
	_snekkja_consumers_id[103] = NULL;\n\
	_snekkja_consumers_id[104] = NULL;\n\
	_snekkja_consumers_id[105] = NULL;\n\
	_snekkja_consumers_id[106] = NULL;\n\
	_snekkja_consumers_id[107] = NULL;\n\
	_snekkja_consumers_id[108] = NULL;\n\
	_snekkja_consumers_id[109] = NULL;\n\
	_snekkja_consumers_id[110] = NULL;\n\
	_snekkja_consumers_id[111] = NULL;\n\
	_snekkja_consumers_id[112] = NULL;\n\
	_snekkja_consumers_id[113] = NULL;\n\
	_snekkja_consumers_id[114] = NULL;\n\
	_snekkja_consumers_id[115] = NULL;\n\
	_snekkja_consumers_id[116] = NULL;\n\
	_snekkja_consumers_id[117] = NULL;\n\
	_snekkja_consumers_id[118] = NULL;\n\
	_snekkja_consumers_id[119] = NULL;\n\
	_snekkja_consumers_id[120] = NULL;\n\
	_snekkja_consumers_id[121] = NULL;\n\
	_snekkja_consumers_id[122] = NULL;\n\
	_snekkja_consumers_id[123] = NULL;\n\
	_snekkja_consumers_id[124] = NULL;\n\
	_snekkja_consumers_id[125] = NULL;\n\
	_snekkja_consumers_id[126] = NULL;\n\
	_snekkja_consumers_id[127] = NULL;\n\
\n\
	_snekkja_schedule = malloc(sizeof(snekkja_schedule_task_t*) * _snekkja_p);\n\
	_snekkja_schedule[0] = malloc(sizeof(snekkja_schedule_task_t) * 1);\n\
	_snekkja_schedule[0][0].id = 3;\n\
	_snekkja_schedule[0][0].start_time = 0;\n\
	_snekkja_schedule[0][0].frequency = 533;\n\
	_snekkja_schedule[1] = malloc(sizeof(snekkja_schedule_task_t) * 2);\n\
	_snekkja_schedule[1][0].id = 5;\n\
	_snekkja_schedule[1][0].start_time = 0;\n\
	_snekkja_schedule[1][0].frequency = 533;\n\
	_snekkja_schedule[1][1].id = 6;\n\
	_snekkja_schedule[1][1].start_time = 1;\n\
	_snekkja_schedule[1][1].frequency = 533;\n\
	_snekkja_schedule[2] = malloc(sizeof(snekkja_schedule_task_t) * 4);\n\
	_snekkja_schedule[2][0].id = 9;\n\
	_snekkja_schedule[2][0].start_time = 0;\n\
	_snekkja_schedule[2][0].frequency = 533;\n\
	_snekkja_schedule[2][1].id = 10;\n\
	_snekkja_schedule[2][1].start_time = 1;\n\
	_snekkja_schedule[2][1].frequency = 533;\n\
	_snekkja_schedule[2][2].id = 11;\n\
	_snekkja_schedule[2][2].start_time = 2;\n\
	_snekkja_schedule[2][2].frequency = 533;\n\
	_snekkja_schedule[2][3].id = 12;\n\
	_snekkja_schedule[2][3].start_time = 3;\n\
	_snekkja_schedule[2][3].frequency = 533;\n\
	_snekkja_schedule[3] = malloc(sizeof(snekkja_schedule_task_t) * 8);\n\
	_snekkja_schedule[3][0].id = 17;\n\
	_snekkja_schedule[3][0].start_time = 0;\n\
	_snekkja_schedule[3][0].frequency = 533;\n\
	_snekkja_schedule[3][1].id = 18;\n\
	_snekkja_schedule[3][1].start_time = 1;\n\
	_snekkja_schedule[3][1].frequency = 533;\n\
	_snekkja_schedule[3][2].id = 19;\n\
	_snekkja_schedule[3][2].start_time = 2;\n\
	_snekkja_schedule[3][2].frequency = 533;\n\
	_snekkja_schedule[3][3].id = 20;\n\
	_snekkja_schedule[3][3].start_time = 3;\n\
	_snekkja_schedule[3][3].frequency = 533;\n\
	_snekkja_schedule[3][4].id = 21;\n\
	_snekkja_schedule[3][4].start_time = 4;\n\
	_snekkja_schedule[3][4].frequency = 533;\n\
	_snekkja_schedule[3][5].id = 22;\n\
	_snekkja_schedule[3][5].start_time = 5;\n\
	_snekkja_schedule[3][5].frequency = 533;\n\
	_snekkja_schedule[3][6].id = 23;\n\
	_snekkja_schedule[3][6].start_time = 6;\n\
	_snekkja_schedule[3][6].frequency = 533;\n\
	_snekkja_schedule[3][7].id = 24;\n\
	_snekkja_schedule[3][7].start_time = 7;\n\
	_snekkja_schedule[3][7].frequency = 533;\n\
	_snekkja_schedule[4] = malloc(sizeof(snekkja_schedule_task_t) * 16);\n\
	_snekkja_schedule[4][0].id = 33;\n\
	_snekkja_schedule[4][0].start_time = 0;\n\
	_snekkja_schedule[4][0].frequency = 533;\n\
	_snekkja_schedule[4][1].id = 34;\n\
	_snekkja_schedule[4][1].start_time = 1;\n\
	_snekkja_schedule[4][1].frequency = 533;\n\
	_snekkja_schedule[4][2].id = 35;\n\
	_snekkja_schedule[4][2].start_time = 2;\n\
	_snekkja_schedule[4][2].frequency = 533;\n\
	_snekkja_schedule[4][3].id = 36;\n\
	_snekkja_schedule[4][3].start_time = 3;\n\
	_snekkja_schedule[4][3].frequency = 533;\n\
	_snekkja_schedule[4][4].id = 37;\n\
	_snekkja_schedule[4][4].start_time = 4;\n\
	_snekkja_schedule[4][4].frequency = 533;\n\
	_snekkja_schedule[4][5].id = 38;\n\
	_snekkja_schedule[4][5].start_time = 5;\n\
	_snekkja_schedule[4][5].frequency = 533;\n\
	_snekkja_schedule[4][6].id = 39;\n\
	_snekkja_schedule[4][6].start_time = 6;\n\
	_snekkja_schedule[4][6].frequency = 533;\n\
	_snekkja_schedule[4][7].id = 40;\n\
	_snekkja_schedule[4][7].start_time = 7;\n\
	_snekkja_schedule[4][7].frequency = 533;\n\
	_snekkja_schedule[4][8].id = 41;\n\
	_snekkja_schedule[4][8].start_time = 8;\n\
	_snekkja_schedule[4][8].frequency = 533;\n\
	_snekkja_schedule[4][9].id = 42;\n\
	_snekkja_schedule[4][9].start_time = 9;\n\
	_snekkja_schedule[4][9].frequency = 533;\n\
	_snekkja_schedule[4][10].id = 43;\n\
	_snekkja_schedule[4][10].start_time = 10;\n\
	_snekkja_schedule[4][10].frequency = 533;\n\
	_snekkja_schedule[4][11].id = 44;\n\
	_snekkja_schedule[4][11].start_time = 11;\n\
	_snekkja_schedule[4][11].frequency = 533;\n\
	_snekkja_schedule[4][12].id = 45;\n\
	_snekkja_schedule[4][12].start_time = 12;\n\
	_snekkja_schedule[4][12].frequency = 533;\n\
	_snekkja_schedule[4][13].id = 46;\n\
	_snekkja_schedule[4][13].start_time = 13;\n\
	_snekkja_schedule[4][13].frequency = 533;\n\
	_snekkja_schedule[4][14].id = 47;\n\
	_snekkja_schedule[4][14].start_time = 14;\n\
	_snekkja_schedule[4][14].frequency = 533;\n\
	_snekkja_schedule[4][15].id = 48;\n\
	_snekkja_schedule[4][15].start_time = 15;\n\
	_snekkja_schedule[4][15].frequency = 533;\n\
	_snekkja_schedule[5] = malloc(sizeof(snekkja_schedule_task_t) * 32);\n\
	_snekkja_schedule[5][0].id = 65;\n\
	_snekkja_schedule[5][0].start_time = 0;\n\
	_snekkja_schedule[5][0].frequency = 533;\n\
	_snekkja_schedule[5][1].id = 66;\n\
	_snekkja_schedule[5][1].start_time = 1;\n\
	_snekkja_schedule[5][1].frequency = 533;\n\
	_snekkja_schedule[5][2].id = 67;\n\
	_snekkja_schedule[5][2].start_time = 2;\n\
	_snekkja_schedule[5][2].frequency = 533;\n\
	_snekkja_schedule[5][3].id = 68;\n\
	_snekkja_schedule[5][3].start_time = 3;\n\
	_snekkja_schedule[5][3].frequency = 533;\n\
	_snekkja_schedule[5][4].id = 69;\n\
	_snekkja_schedule[5][4].start_time = 4;\n\
	_snekkja_schedule[5][4].frequency = 533;\n\
	_snekkja_schedule[5][5].id = 70;\n\
	_snekkja_schedule[5][5].start_time = 5;\n\
	_snekkja_schedule[5][5].frequency = 533;\n\
	_snekkja_schedule[5][6].id = 71;\n\
	_snekkja_schedule[5][6].start_time = 6;\n\
	_snekkja_schedule[5][6].frequency = 533;\n\
	_snekkja_schedule[5][7].id = 72;\n\
	_snekkja_schedule[5][7].start_time = 7;\n\
	_snekkja_schedule[5][7].frequency = 533;\n\
	_snekkja_schedule[5][8].id = 73;\n\
	_snekkja_schedule[5][8].start_time = 8;\n\
	_snekkja_schedule[5][8].frequency = 533;\n\
	_snekkja_schedule[5][9].id = 74;\n\
	_snekkja_schedule[5][9].start_time = 9;\n\
	_snekkja_schedule[5][9].frequency = 533;\n\
	_snekkja_schedule[5][10].id = 75;\n\
	_snekkja_schedule[5][10].start_time = 10;\n\
	_snekkja_schedule[5][10].frequency = 533;\n\
	_snekkja_schedule[5][11].id = 76;\n\
	_snekkja_schedule[5][11].start_time = 11;\n\
	_snekkja_schedule[5][11].frequency = 533;\n\
	_snekkja_schedule[5][12].id = 77;\n\
	_snekkja_schedule[5][12].start_time = 12;\n\
	_snekkja_schedule[5][12].frequency = 533;\n\
	_snekkja_schedule[5][13].id = 78;\n\
	_snekkja_schedule[5][13].start_time = 13;\n\
	_snekkja_schedule[5][13].frequency = 533;\n\
	_snekkja_schedule[5][14].id = 79;\n\
	_snekkja_schedule[5][14].start_time = 14;\n\
	_snekkja_schedule[5][14].frequency = 533;\n\
	_snekkja_schedule[5][15].id = 80;\n\
	_snekkja_schedule[5][15].start_time = 15;\n\
	_snekkja_schedule[5][15].frequency = 533;\n\
	_snekkja_schedule[5][16].id = 81;\n\
	_snekkja_schedule[5][16].start_time = 16;\n\
	_snekkja_schedule[5][16].frequency = 533;\n\
	_snekkja_schedule[5][17].id = 82;\n\
	_snekkja_schedule[5][17].start_time = 17;\n\
	_snekkja_schedule[5][17].frequency = 533;\n\
	_snekkja_schedule[5][18].id = 83;\n\
	_snekkja_schedule[5][18].start_time = 18;\n\
	_snekkja_schedule[5][18].frequency = 533;\n\
	_snekkja_schedule[5][19].id = 84;\n\
	_snekkja_schedule[5][19].start_time = 19;\n\
	_snekkja_schedule[5][19].frequency = 533;\n\
	_snekkja_schedule[5][20].id = 85;\n\
	_snekkja_schedule[5][20].start_time = 20;\n\
	_snekkja_schedule[5][20].frequency = 533;\n\
	_snekkja_schedule[5][21].id = 86;\n\
	_snekkja_schedule[5][21].start_time = 21;\n\
	_snekkja_schedule[5][21].frequency = 533;\n\
	_snekkja_schedule[5][22].id = 87;\n\
	_snekkja_schedule[5][22].start_time = 22;\n\
	_snekkja_schedule[5][22].frequency = 533;\n\
	_snekkja_schedule[5][23].id = 88;\n\
	_snekkja_schedule[5][23].start_time = 23;\n\
	_snekkja_schedule[5][23].frequency = 533;\n\
	_snekkja_schedule[5][24].id = 89;\n\
	_snekkja_schedule[5][24].start_time = 24;\n\
	_snekkja_schedule[5][24].frequency = 533;\n\
	_snekkja_schedule[5][25].id = 90;\n\
	_snekkja_schedule[5][25].start_time = 25;\n\
	_snekkja_schedule[5][25].frequency = 533;\n\
	_snekkja_schedule[5][26].id = 91;\n\
	_snekkja_schedule[5][26].start_time = 26;\n\
	_snekkja_schedule[5][26].frequency = 533;\n\
	_snekkja_schedule[5][27].id = 92;\n\
	_snekkja_schedule[5][27].start_time = 27;\n\
	_snekkja_schedule[5][27].frequency = 533;\n\
	_snekkja_schedule[5][28].id = 93;\n\
	_snekkja_schedule[5][28].start_time = 28;\n\
	_snekkja_schedule[5][28].frequency = 533;\n\
	_snekkja_schedule[5][29].id = 94;\n\
	_snekkja_schedule[5][29].start_time = 29;\n\
	_snekkja_schedule[5][29].frequency = 533;\n\
	_snekkja_schedule[5][30].id = 95;\n\
	_snekkja_schedule[5][30].start_time = 30;\n\
	_snekkja_schedule[5][30].frequency = 533;\n\
	_snekkja_schedule[5][31].id = 96;\n\
	_snekkja_schedule[5][31].start_time = 31;\n\
	_snekkja_schedule[5][31].frequency = 533;\n\
}\n\
\n\
void snekkja_schedule_destroy()\n\
{\n\
	free(_snekkja_schedule[5]);\n\
	free(_snekkja_schedule[4]);\n\
	free(_snekkja_schedule[3]);\n\
	free(_snekkja_schedule[2]);\n\
	free(_snekkja_schedule[1]);\n\
	free(_snekkja_schedule[0]);\n\
	free(_snekkja_schedule);\n\
	free(_snekkja_consumers_id[95]);\n\
	free(_snekkja_consumers_id[94]);\n\
	free(_snekkja_consumers_id[93]);\n\
	free(_snekkja_consumers_id[92]);\n\
	free(_snekkja_consumers_id[91]);\n\
	free(_snekkja_consumers_id[90]);\n\
	free(_snekkja_consumers_id[89]);\n\
	free(_snekkja_consumers_id[88]);\n\
	free(_snekkja_consumers_id[87]);\n\
	free(_snekkja_consumers_id[86]);\n\
	free(_snekkja_consumers_id[85]);\n\
	free(_snekkja_consumers_id[84]);\n\
	free(_snekkja_consumers_id[83]);\n\
	free(_snekkja_consumers_id[82]);\n\
	free(_snekkja_consumers_id[81]);\n\
	free(_snekkja_consumers_id[80]);\n\
	free(_snekkja_consumers_id[79]);\n\
	free(_snekkja_consumers_id[78]);\n\
	free(_snekkja_consumers_id[77]);\n\
	free(_snekkja_consumers_id[76]);\n\
	free(_snekkja_consumers_id[75]);\n\
	free(_snekkja_consumers_id[74]);\n\
	free(_snekkja_consumers_id[73]);\n\
	free(_snekkja_consumers_id[72]);\n\
	free(_snekkja_consumers_id[71]);\n\
	free(_snekkja_consumers_id[70]);\n\
	free(_snekkja_consumers_id[69]);\n\
	free(_snekkja_consumers_id[68]);\n\
	free(_snekkja_consumers_id[67]);\n\
	free(_snekkja_consumers_id[66]);\n\
	free(_snekkja_consumers_id[65]);\n\
	free(_snekkja_consumers_id[64]);\n\
	free(_snekkja_consumers_id[47]);\n\
	free(_snekkja_consumers_id[46]);\n\
	free(_snekkja_consumers_id[45]);\n\
	free(_snekkja_consumers_id[44]);\n\
	free(_snekkja_consumers_id[43]);\n\
	free(_snekkja_consumers_id[42]);\n\
	free(_snekkja_consumers_id[41]);\n\
	free(_snekkja_consumers_id[40]);\n\
	free(_snekkja_consumers_id[39]);\n\
	free(_snekkja_consumers_id[38]);\n\
	free(_snekkja_consumers_id[37]);\n\
	free(_snekkja_consumers_id[36]);\n\
	free(_snekkja_consumers_id[35]);\n\
	free(_snekkja_consumers_id[34]);\n\
	free(_snekkja_consumers_id[33]);\n\
	free(_snekkja_consumers_id[32]);\n\
	free(_snekkja_consumers_id[23]);\n\
	free(_snekkja_consumers_id[22]);\n\
	free(_snekkja_consumers_id[21]);\n\
	free(_snekkja_consumers_id[20]);\n\
	free(_snekkja_consumers_id[19]);\n\
	free(_snekkja_consumers_id[18]);\n\
	free(_snekkja_consumers_id[17]);\n\
	free(_snekkja_consumers_id[16]);\n\
	free(_snekkja_consumers_id[11]);\n\
	free(_snekkja_consumers_id[10]);\n\
	free(_snekkja_consumers_id[9]);\n\
	free(_snekkja_consumers_id[8]);\n\
	free(_snekkja_consumers_id[5]);\n\
	free(_snekkja_consumers_id[4]);\n\
	free(_snekkja_consumers_id);\n\
	free(_snekkja_producers_id[47]);\n\
	free(_snekkja_producers_id[46]);\n\
	free(_snekkja_producers_id[45]);\n\
	free(_snekkja_producers_id[44]);\n\
	free(_snekkja_producers_id[43]);\n\
	free(_snekkja_producers_id[42]);\n\
	free(_snekkja_producers_id[41]);\n\
	free(_snekkja_producers_id[40]);\n\
	free(_snekkja_producers_id[39]);\n\
	free(_snekkja_producers_id[38]);\n\
	free(_snekkja_producers_id[37]);\n\
	free(_snekkja_producers_id[36]);\n\
	free(_snekkja_producers_id[35]);\n\
	free(_snekkja_producers_id[34]);\n\
	free(_snekkja_producers_id[33]);\n\
	free(_snekkja_producers_id[32]);\n\
	free(_snekkja_producers_id[23]);\n\
	free(_snekkja_producers_id[22]);\n\
	free(_snekkja_producers_id[21]);\n\
	free(_snekkja_producers_id[20]);\n\
	free(_snekkja_producers_id[19]);\n\
	free(_snekkja_producers_id[18]);\n\
	free(_snekkja_producers_id[17]);\n\
	free(_snekkja_producers_id[16]);\n\
	free(_snekkja_producers_id[11]);\n\
	free(_snekkja_producers_id[10]);\n\
	free(_snekkja_producers_id[9]);\n\
	free(_snekkja_producers_id[8]);\n\
	free(_snekkja_producers_id[5]);\n\
	free(_snekkja_producers_id[4]);\n\
	free(_snekkja_producers_id[2]);\n\
	free(_snekkja_producers_id);\n\
	free(_snekkja_remote_producers_in_task);\n\
	free(_snekkja_remote_consumers_in_task);\n\
	free(_snekkja_producers_in_task);\n\
	free(_snekkja_consumers_in_task);\n\
	free(_snekkja_producers_in_core);\n\
	free(_snekkja_consumers_in_core);\n\
	free(_snekkja_tasks_in_core);\n\
}\n\
\n\
void*\n\
snekkja_function(size_t id, task_status_t status)\n\
{\n\
	switch(id)\n\
	{\n\
		default:\n\
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
