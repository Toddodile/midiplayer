#ifndef SHARED_DOUBLE_BUFFER_HPP
#define SHARED_DOUBLE_BUFFER_HPP

#include <cstdlib>
#include <cstdint>
#include <mutex>
#include <queue>

typedef std::queue<std::int16_t> DataQueue;

class SharedDoubleBuffer {
public:

	SharedDoubleBuffer(std::size_t size = 8192);

	std::int16_t pop();

	bool tryPush(std::int16_t data);

	void clear();

private:
	std::size_t maxSize;

	std::mutex queueMutex;

	DataQueue input;
	DataQueue output;

};

#endif // !SHARED_DOUBLE_BUFFER_HPP
