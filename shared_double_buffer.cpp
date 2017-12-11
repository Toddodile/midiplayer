#include "shared_double_buffer.hpp"

SharedDoubleBuffer::SharedDoubleBuffer(std::size_t size)
{
	maxSize = size;
	input = std::queue<std::int16_t>();
	output = std::queue<std::int16_t>();
}

std::int16_t SharedDoubleBuffer::pop()
{
	if (output.size() > 0)
	{
		std::int16_t data = output.front();
		output.pop();
		return data;
	}
	std::lock_guard<std::mutex> lock(queueMutex);
	if (input.size() == 0)
	{
		return 0;
	}
	DataQueue temp = input;
	input = output;
	output = temp;
	std::int16_t data = output.front();
	output.pop();
	return data;
}

bool SharedDoubleBuffer::tryPush(std::int16_t data)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	if (input.size() < maxSize)
	{
		input.emplace(data);
		return true;
	}
	return false;
}
