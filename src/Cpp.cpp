#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Cpp.hpp>
#include <CppScalar.hpp>
#include <CppVector.hpp>
#include <CppSet.hpp>
#include <CppMatrix.hpp>

#include <ParseException.hpp>
#include <CastException.hpp>

namespace pelib
{
	Cpp::Cpp()
	{				
		// Add interpreters
		addOutputs();
	}

	Cpp::Cpp(std::vector<CppData*> outputs)
	{
		this->outputs = outputs;
	}

	Cpp::~Cpp()
	{
		deleteOutputs();			
	}

	void
	Cpp::deleteOutputs()
	{
		for(std::vector<CppData*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	Cpp&
	Cpp::operator=(const Cpp &amplOutput)
	{
		deleteOutputs();

		for(std::vector<CppData*>::const_iterator i = amplOutput.outputs.begin(); i != amplOutput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	void
	Cpp::dump(std::ostream& o, const Record &record) const
	{
		o << "pelib::Record" << std::endl << "make_record()" << std::endl << "{" << std::endl;
		o << "pelib::Record new_record;" << std::endl;
		
		std::map<std::string, const Data * const> records = record.getAllRecords();
		for (std::map<std::string, const Data * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			dump(o, rec->second);
			o << "new_record.insert(&" << rec->first << ");" << std::endl;
		}

		o << "return new_record;" << std::endl;
		o << "}" << std::endl;
	}

	void
	Cpp::dump(std::ostream& o, const Data *data) const
	{
		for (std::vector<CppData*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
		{
			const DataOutput *output = *out;
			try
			{
				output->dump(o, data);
				break;
			} catch(CastException &e)
			{
				// No suitable element to output
				// Couldn't cast the element to record: just let that go and try again with next element
			}
		}
	}

	void			
	Cpp::addOutputs()
	{		
		outputs.push_back(new CppScalar<int>());
		outputs.push_back(new CppScalar<float>());
		outputs.push_back(new CppVector<int, int>());
		outputs.push_back(new CppVector<int, float>());
		outputs.push_back(new CppSet<int>());
		outputs.push_back(new CppSet<float>());
		outputs.push_back(new CppMatrix<int, int, int>());
		outputs.push_back(new CppMatrix<int, int, float>());
	}
}
