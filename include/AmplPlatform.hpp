#include <Platform.hpp>

#include <AmplPlatformParser.hpp>
#include <AmplPlatformOutput.hpp>

#ifndef PELIB_AMPLPLATFORM
#define PELIB_AMPLPLATFORM

namespace pelib
{
	class AmplPlatform : public AmplPlatformParser, public AmplPlatformOutput
	{
		public:
			virtual Platform* parse(std::istream &is) const;
			virtual	void dump(std::ostream &os, const Record *data) const;
			virtual	void dump(std::ostream &os, const Record &data) const;
			virtual AmplPlatform* clone() const;
	};
}

#endif
