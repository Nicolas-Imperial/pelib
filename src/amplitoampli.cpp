#include <Task.hpp>
#include <set>

#include <AmplInput.hpp>

using namespace pelib;

/*
class B
{
	public:
	// Compare two tasks by their starting time
	struct compare_by_start_time : std::binary_function <B, B, bool>
	{
		bool
		operator()(const B& x, const B& y) const
		{		
			return x.getStartTime() < y.getStartTime();
		}
	};

	float getStartTime() const;
};

class A
{
	public:
		std::set<B, B::compare_by_start_time>& getSet() { return hello; }
		void setSet(std::set<B, B::compare_by_start_time> &myset) { hello = myset; }

	protected:
		std::set<B, B::compare_by_start_time> hello;
};
*/

int
main(int argc, char **argv)
{
	AmplInput input(AmplInput::intFloatHandlers());

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	input.dump(std::cout, input.parse(std::cin));

	return EXIT_SUCCESS;
}

