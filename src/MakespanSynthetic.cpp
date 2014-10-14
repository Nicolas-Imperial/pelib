#include "MakespanSynthetic.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanSynthetic::calculate(const Taskgraph& tg, const Platform& arch) const
{
	int p = arch.getCoreNumber();
	int min_freq = *arch.getFrequencies().begin();
	int max_freq = *arch.getFrequencies().rbegin();

	double pseudo_seq = 0;
	double parallel = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		Task task = *i;
		double wi = task.getMaxWidth();
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than platform offers, even if width is higher
		double pseudo_seq_time = task.runtime(1, 1) / task.getEfficiency(wi); // Assume tasks run sequentially, but pay the parallelization penatly; actual parallelization comes later
		double parallel_time = task.runtime(wi, 1); // Assume tasks run sequentially, but pay the parallelization penatly; actual parallelization comes later
		pseudo_seq += pseudo_seq_time;
		parallel += parallel_time; 
	}

	// A perfectly load-balanced schedule would be
	float perfect = pseudo_seq / p;

	// Take the workload as an average between a perfect and the worst (all mapped on one core) schedule
	parallel = (3 * perfect + parallel) / 4;

	float minM = parallel / (double)max_freq;
	float maxM = parallel / (double)min_freq;
	float M = (minM + maxM) / 2;

	return M;
}
