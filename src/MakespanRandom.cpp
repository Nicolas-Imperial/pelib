#include "MakespanRandom.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

using namespace pelib;
using namespace std;

double MakespanRandom::calculate(const Record& tasks,const Record& architecture) const
{

  //int n = tasks.find<Scalar<int> >("n")->getValue();
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
	return M;
}
