#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_FORMULA
#define MAKESPAN_FORMULA

namespace pelib
{
  class MakespanFormula : public MakespanCalculator
  {
	public:
		MakespanFormula(string formula);
		virtual double calculate(const Taskgraph& tg,const Architecture& arch) const;
		  
	private:
    	const string formula;
  };
}



#endif
