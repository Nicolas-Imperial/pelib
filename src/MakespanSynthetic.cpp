#include "MakespanSynthetic.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double
MakespanSynthetic::calculate(const Taskgraph& tg, const Architecture& arch) const
{
	int p = arch.getCoreNumber();
	int min_freq = *arch.getFrequencies().begin();
	int max_freq = *arch.getFrequencies().end();

	double sum_pTw = 0;
	for(std::set<Task>::const_iterator i = tg.getTasks().begin(); i != tg.getTasks().end(); i++)
	{
		Task task = *i;
		double wi = task.getMaxWidth();
		wi = wi < p ? wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher
		double time = task.runtime(wi, 1);
		sum_pTw += time;
	}

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	
	return M;

	/*
	const Matrix<int, int, float> *e = tasks.find<Matrix<int, int, float> >("e");
	const Vector<int, int> *tau = tasks.find<Vector<int, int> >("Tau");
	double sum_pTw = 0;
	double temp;
	const Vector<int, int> *Wi = tasks.find<Vector<int, int> >("Wi");
	map<int, int> width = Wi->getValues();

	for(std::map<int, int>::const_iterator i = width.begin(); i != width.end(); i++)
	{
		int task = i->first;
		temp = tau->find(task);
		temp_wi = i->second;
		temp_wi = temp_wi < p ? temp_wi : p; // Cannot have a task running with more cores than architecture offers, even if width is higher 
		temp_e = e->find(temp_wi, task);
		temp_e = temp_e * (double)temp_wi;
		temp = temp / temp_e;
		sum_pTw += temp;
	}

	float minM = sum_pTw / (double)max_freq;
	float maxM = sum_pTw / (double)min_freq;
	float M = (minM + maxM) / 2;
	*/
}
