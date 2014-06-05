#include "MakespanFFT.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanFFT::calculate(const Taskgraph &tg, const Architecture &arch) const
{
	//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] tg.getTasks().size() = " << tg.getTasks().size() << endl;
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
		//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << endl;

		if(task.getWorkload() == max_workload)
		{
			// Skip the root task, the one whose workload is highest 
			continue;
		}
		
		double wi = task.getMaxWidth();
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher
		double time = task.runtime(1, 1) / wi;
		//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] wi = " << wi << endl;
		sum_pTw += time;
	}
	//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] sum_pTw = " << sum_pTw << endl;


	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	
	return M;
}
