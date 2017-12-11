#ifndef SHARED_DOUBLE_BUFFER_HPP
#define SHARED_DOUBLE_BUFFER_HPP

#include <cstdlib>
#include <cstdint>
#include <mutex>
#include <queue>

typedef std::queue<std::int16_t> DataQueue;

class SharedDoubleBuffer {
public:

	/**
	* Creates a SharedDoubleBuffer with the given size internal
	* queues
	* \param size The size of the internal queues
	*/
	SharedDoubleBuffer(std::size_t size = 8192);

	/**
	* Gets the front of the output queue. 0 if buffer is empty
	*/
	std::int16_t pop();

	/**
	* Trys to push the given data to the input queue
	* \param data The sample to push
	* \return True if push was successful, false otherwise
	*/
	bool tryPush(std::int16_t data);

	/*
	* Clears the contents of the queue
	*/
	void clear();

	/*
	* Returns the maximum size of the internal queues
	* \return The maximum size of the internal queues
	*/
	std::size_t getMaxSize();

	/*
	* Whether or not the buffer has values in it
	* \return true if there are no values in the buffer
	*/
	bool isEmpty();

private:
	std::size_t maxSize;

	std::mutex queueMutex;

	DataQueue input;
	DataQueue output;

};

#endif // !SHARED_DOUBLE_BUFFER_HPP
