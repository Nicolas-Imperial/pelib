#include <Architecture.hpp>

using namespace std;

namespace pelib
{
	Architecture::Architecture(const std::string name) : StreamingAppData(name) { /* Do nothing */ };
	Architecture::Architecture(const Architecture *arch) : StreamingAppData(arch->getName()) { /* Do nothing */ }
	Architecture*
	Architecture::clone() const
	{
		return new Architecture(name);
	}
}
