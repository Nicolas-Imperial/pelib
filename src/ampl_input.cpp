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


#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/AmplInput.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#if 0
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#else
#define debug(var)
#endif
#endif

typedef struct
{
	vector<string> include;
	vector<string> exclude;
} filter_t;

#if 0
static filter_t
parse_args(char **argv)
{
	filter_t filter;
	while((void*)argv[0] != NULL)
	{
		if(string(*argv).compare("--include") == 0)
		{
			while(string(*argv).compare("--") != 0 && (void*)argv != NULL)
			{
				filter.include.insert(filter.include.begin(), string(*argv));
				argv++;
			}
		}

		if(string(*argv).compare("--include") == 0)
		{
			while(string(*argv).compare("--") != 0 && (void*)argv != NULL)
			{
				filter.include.insert(filter.exclude.begin(), string(*argv));
			}
		}
	}

	return filter;
}
#endif

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv)
{
	
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
	Algebra al = AmplInput(AmplInput::intFloatHandlers()).parse(cin);
	return new Algebra(al);
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Algebra al = *(Algebra*)(records.find(typeid(Algebra).name())->second);
	AmplInput(AmplInput::intFloatHandlers()).dump(cout, al);
}

#ifdef __cplusplus
}
#endif

