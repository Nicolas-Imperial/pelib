#include "MakespanFFT.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanFFT::calculate(const Taskgraph& tg,const Architecture& arch) const
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
		
		double wi = task.getMaxWidth();
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher
		double time = task.runtime(1, 1) / p;
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

		temp_wi = i->second;
		//temp_wi = p;
		temp_wi = temp_wi < p ? temp_wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher 
		temp_e = e->find(temp_wi, task);
		//temp_e = temp_e * (double)p; // for tasks of a level whose width is higher than p, we can divide by p; for tasks of a level whose width is lower than p, take as much tasks in this level divided by how bigger is p than the width of this level and divide by max width (temp_wi), or to simplify, take all tasks and divide their work by p even if p is higher than the width; it evens out on average
		// Ignore efficiency loss with parallelization
		temp_e = (double)p; // for tasks of a level whose width is higher than p, we can divide by p; for tasks of a level whose width is lower than p, take as much tasks in this level divided by how bigger is p than the width of this level and divide by max width (temp_wi), or to simplify, take all tasks and divide their work by p even if p is higher than the width; it evens out on average
		temp = temp / temp_e;
		sum_pTw += temp;
	}
//	float minM = sum_pTw / (double)max_freq;
//	float maxM = sum_pTw / (double)min_freq;
//	float M = (minM + maxM) / 2;
	float M = (sum_pTw / (float)max_freq + sum_pTw / (float)min_freq) / 2;
	if(M == (int)M)
	{
		M = M + 0.0001;
	}

	return M;
*/
}
