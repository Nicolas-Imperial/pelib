#include <Architecture.hpp>

using namespace std;

namespace pelib
{
	Architecture::Architecture() { /* Do nothing */ };
	Architecture::Architecture(const Architecture *arch) { /* Do nothing */ }
	Architecture*
	Architecture::clone() const
	{
		return new Architecture();
	}
}
