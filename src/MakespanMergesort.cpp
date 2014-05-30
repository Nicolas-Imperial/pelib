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

double MakespanMergesort::calculate(const Taskgraph& tg,const Architecture& arch) const
{
	int p = arch.getCoreNumber();
	int min_freq = *arch.getFrequencies().begin();
	int max_freq = *arch.getFrequencies().end();

	set<int> workloads;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		workloads.insert(i->getWorkload());
	}
	int max_workload = *workloads.end();

	double sum_pTw = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		Task task = *i;

		if(task.getWorkload() == max_workload)
		{
			// Skip the root task, the one whose workload is highest 
			continue;
		}
		
		double wi = number_tasks_in_level(task.getWorkload() / max_workload);
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher
		double time = task.runtime(1, 1) / wi;
		sum_pTw += time;
	}

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	
	return M;

	/*
	int p = architecture.find<Scalar<int> >("p")->getValue();
	int min_freq = *architecture.find<Set<int> >("F")->getValues().begin();
	int max_freq = *architecture.find<Set<int> >("F")->getValues().end();
	int task;
	int temp_wi;
	double temp_e;
	const Matrix<int, int, float> *e = tasks.find<Matrix<int, int, float> >("e");
	const Vector<int, int> *tau = tasks.find<Vector<int, int> >("Tau");
	double sum_pTw = 0;
	double temp;
	const Vector<int, int> *Wi = tasks.find<Vector<int, int> >("Wi");
	map<int, int> width = Wi->getValues();

	for(std::map<int, int>::const_iterator i = width.begin(); i != width.end(); i++)
	{
		task = i->first;
		if(task == 1)
		{
			continue;
		}
		temp = tau->find(task);

		temp_wi = number_tasks_in_level(task); // Every task are sequential, but we add all tasks 's average work assuming they were perfectly distributed among core
		//temp_wi = p;
		temp_wi = temp_wi < p ? temp_wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher 
		temp_e = e->find(temp_wi, task);
		// Ignore efficiency loss with parallelization
		temp_e = (double)temp_wi;
		temp = temp / temp_e;
		sum_pTw += temp;
	//	printf("%f\n", sum_pTw);
	}
	float M = (sum_pTw / (float)max_freq + sum_pTw / (float)min_freq) / 2;
	if(M == (int) M)
	{
		M = M + 0.0001;
	}
*/

	return M;
}
