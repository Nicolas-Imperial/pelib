#include <Matrix.hpp>
#include <Algebra.hpp>
#include <AmplOutput.hpp>

using namespace pelib;
using namespace std;

int
main(int argc, char **argv)
{
	AmplOutput input;

	if(argc < 2)
	{
		cerr << "usage: cat <ampl file .dat> | " << argv[0] << " <matrix name>" << endl;
	}

	// Set floating point var output format to fixed at 7 digits
	//cout << setprecision(6)
 	//<< setiosflags(ios::fixed)                                            
	//<< setiosflags(ios::showpoint);

	Algebra al = input.parse(cin);

	map<int, map<int, float> > record = al.find<Matrix<int, int, float> >(argv[1])->getValues();

	//AmplOutput().dump(cerr, al.find<Matrix<int, int, float> >(argv[1]));

	for(map<int, map<int, float> >::const_iterator i = record.begin(); i != record.end(); i++)
	{
		for(map<int, float>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			cout << j->second << " ";
			//cerr << i->first << " " << j->first << " " << j->second << " ";
		}
		
		cout << endl;
	}

	return EXIT_SUCCESS;
}
