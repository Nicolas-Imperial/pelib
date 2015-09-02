/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <pelib/Matrix.hpp>
#include <pelib/Algebra.hpp>
#include <pelib/AmplOutput.hpp>

#ifndef debug
#if 0
#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;
#else
#define debug(var)
#endif
#endif

using namespace pelib;
using namespace std;

int
main(int argc, char **argv)
{
	AmplOutput input(AmplOutput::stringHandlers());

	if(argc < 2)
	{
		cerr << "usage: cat <ampl file .dat> | " << argv[0] << " <matrix name>" << endl;
	}

	// Set floating point var output format to fixed at 7 digits
	//cout << setprecision(6)
 	//<< setiosflags(ios::fixed)                                            
	//<< setiosflags(ios::showpoint);

	Algebra al = input.parse(cin);

	map<int, map<int, string> > record = al.find<Matrix<int, int, string> >(argv[1])->getValues();
	//AmplOutput().dump(cerr, al.find<Matrix<int, int, float> >(argv[1]));

	for(map<int, map<int, string> >::const_iterator i = record.begin(); i != record.end(); i++)
	{
		for(map<int, string>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			cout << j->second << " ";
			//cerr << i->first << " " << j->first << " " << j->second << " ";
		}
		
		cout << endl;
	}

	return EXIT_SUCCESS;
}
