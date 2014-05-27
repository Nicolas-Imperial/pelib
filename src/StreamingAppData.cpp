#include <iostream>

#include <StreamingAppData.hpp>

namespace pelib
{
	StreamingAppData::StreamingAppData(const std::string new_name): Data(new_name) {}
	StreamingAppData::StreamingAppData(const StreamingAppData &original): Data(original) {}
}
