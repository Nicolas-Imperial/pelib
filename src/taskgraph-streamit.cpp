#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <AmplOutput.hpp>
#include <AmplInput.hpp>
#include <Matrix.hpp>
#include <Task.hpp>
#include <Link.hpp>
#include <Taskgraph.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv)
{
	using namespace std;
	using namespace pelib;

	AmplOutput ampl_output(AmplOutput::intFloatHandlers());
	AmplInput ai(AmplInput::floatHandlers());

	std::cout << std::setprecision(6)
		<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);

	Algebra data = ai.parse(cin);
	const Matrix<int, int, float> *taskworkcomm = data.find<Matrix<int, int, float> >("taskworkcomm");
	set<Task> tasks;
	set<Link> links;

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
	Taskgraph *tg = new Taskgraph(tasks, links);
	tg->setName(autname);
	tg->setMakespanCalculator("class:synthetic");
// We don't care about any argument here
#if 0
	while(*argv != NULL)
	{
		cout << *argv << " ";
		argv++;
	}
	cout << endl;
#endif

//	cout << cin.rdbuf();
	return tg;
}

void
pelib_delete(Record *rec)
{
	delete rec;
}

#ifdef __cplusplus
}
#endif

