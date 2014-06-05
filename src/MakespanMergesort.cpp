#include "MakespanMergesort.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#include <set>

using namespace pelib;
using namespace std;

static int
number_tasks_in_level(int task)
{
	/*
	1 -> 1
	2 -> 2
	3 -> 2
	4 -> 4
	5 -> 4
	6 -> 4
	7 -> 4
	8 -> 8
	*/
	return pow(2, floor(log(task) / log(2)));
}

double MakespanMergesort::calculate(const Taskgraph& tg, const Architecture& arch) const
{
	int p = arch.getCoreNumber();
	int min_freq = *arch.getFrequencies().begin();
	int max_freq = *arch.getFrequencies().end();

	int max_workload = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		int workload = i->getWorkload();
		if(workload > max_workload)
		{
			max_workload = workload;
		}
	}

	double sum_pTw = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		Task task = *i;

		if(task.getWorkload() == max_workload)
		{
			// Skip the root task, the one whose workload is highest 
			continue;
		}
		
		double wi = number_tasks_in_level(max_workload / task.getWorkload());
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher
		double time = task.runtime(1, 1) / wi;
		sum_pTw += time;
	}

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	
	return M;
}
