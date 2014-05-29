#include <Task.hpp>

namespace pelib
{
	Task::Task(std::string id)
	{
		frequency = 0;
		this->id = id;
	}
			
	int
	Task::getFrequency() const
	{
		return frequency;
	}

	void
	Task::setFrequency(int frequency)
	{
		this->frequency = frequency; 
	}

	std::string
	Task::getId() const
	{
		return this->id;
	}
}
