#include <AmplOutput.hpp>
#include <GraphML.hpp>
#include <XMLSchedule.hpp>
#include <AmplPlatform.hpp>

#include <fstream>

using namespace pelib;
using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		cerr << "Need a taskgraph and platform to perform this task" << endl;
		return 1;
	}

	ifstream tg_str(argv[1]);
	Taskgraph tg = GraphML().parse(tg_str);
	tg_str.close();

	ifstream arch_str(argv[2]);
	Platform arch = AmplPlatform().parse(arch_str);
	arch_str.close();

	/*
	ifstream sched_str(argv[3]);
	Algebra amplschedule = AmplOutput().parse(sched_str);
	sched_str.close();
	*/
	Algebra amplschedule = AmplOutput(AmplOutput::floatHandlers()).parse(cin);
	
	amplschedule = amplschedule.merge(tg.buildAlgebra(arch));
	amplschedule = amplschedule.merge(arch.buildAlgebra());
	
	XMLSchedule().dump(cout, Schedule("converted_from_ampl", tg, amplschedule));

	return 0;
}

  
