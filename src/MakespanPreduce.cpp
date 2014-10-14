#include "MakespanPreduce.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanPreduce::calculate(const Taskgraph& tg,const Platform& arch) const
{
	int p = arch.getCoreNumber();
	int min_freq = *arch.getFrequencies().begin();
	int max_freq = *arch.getFrequencies().rbegin();

	double sum_pTw = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		Task task = *i;
		double wi = task.getMaxWidth();
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than platform offers, even if width is higher
		double time = task.runtime(1, 1) / wi; // We assume perfect scalability here
		sum_pTw += time;
	}

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	
	return M;
}
