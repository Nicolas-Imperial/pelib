#include <pelib/DeadlineCalculator.hpp>
#include <pelib/Algebra.hpp>

#ifndef DEADLINE_FORMULA
#define DEADLINE_FORMULA

namespace pelib
{
  class DeadlineFormula : public DeadlineCalculator
  {
	public:
		DeadlineFormula(string formula);
		virtual double calculate(const Taskgraph& tg,const Platform& arch) const;
		  
	private:
    	const string formula;
  };
}



#endif
