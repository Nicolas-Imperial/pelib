#include "MakespanFFT.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanFFT::calculate(const Taskgraph &tg, const Platform &arch) const
{
	//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] tg.getTasks().size() = " << tg.getTasks().size() << endl;
	int p = arch.getCoreNumber();
	float min_freq = *arch.getFrequencies().begin();
	float max_freq = *arch.getFrequencies().rbegin();

	float max_workload = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		float workload = i->getWorkload();
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
		wi = p; // We assume perfect parallelization, no matter what
		double time = task.runtime(1, 1) / wi;
		//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] wi = " << wi << endl;
		sum_pTw += time;
	}
	//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] sum_pTw = " << sum_pTw << endl;

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] minM = " << minM << "; maxM = " << maxM << endl;
	float M = (minM + maxM) / 2;
	
	return M;
}
