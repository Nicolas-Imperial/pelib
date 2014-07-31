#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>
#include <Algebra.hpp>
#include <Matrix.hpp>
#include <GraphML.hpp>
#include <Taskgraph.hpp>
#include <Task.hpp>

int main(int argc, char **argv)
{
	using namespace std;
	using namespace pelib;

	GraphML tg_output;
	AmplOutput ampl_output(AmplOutput::intFloatHandlers());
	AmplInput ai(AmplInput::floatHandlers());

	std::cout << std::setprecision(6)
		<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);

	Algebra data = ai.parse(cin);
	Taskgraph tg;
	const Matrix<int, int, float> *taskworkcomm = data.find<Matrix<int, int, float> >("taskworkcomm");
	set<Task> tasks;

	for(map<int, map<int, float> >::const_iterator i = taskworkcomm->getValues().begin(); i != taskworkcomm->getValues().end(); i++)
	{
		Task t(i->first);
		stringstream efficiency;

		float work = i->second.find(1)->second;
		float comm = i->second.find(2)->second;

		t.setWorkload(work);
		efficiency << "fml: p == 1 ? 1 : p <= W ? tau / (tau + p * ";
		if(isfinite(comm))
		{
			t.setMaxWidth(std::numeric_limits<double>::infinity());
			efficiency << comm;
		}
		else
		{
			t.setMaxWidth(1);
			efficiency << std::numeric_limits<float>::max();
		}
		efficiency << ") : 1e-6";
		t.setEfficiencyString(efficiency.str());

		tasks.insert(t);
	}

	char* autname;
	if(argc > 1)
	{
		autname = argv[1];
	}
	else
	{
		autname = (char*)string("converted_from_streamit").c_str();
	}
	tg.setAUTName(autname);
	tg.setMakespanCalculator("class:synthetic");
	tg.setTasks(tasks);

	//ampl_output.dump(cout, data);
	tg_output.dump(cout, tg);

	return 0;
}
