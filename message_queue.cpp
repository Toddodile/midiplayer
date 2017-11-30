#include "message_queue.hpp"
#include <mutex>

MessageQueue::MessageQueue()
{
	messages = std::queue<Message>();
}

Message MessageQueue::pop()
{
	std::lock_guard<std::mutex> lock(messageMutex);
	if (messages.empty())
	{
		return Message(Message::EMPTY);
	}
	Message temp = messages.front();
	messages.pop();
	return temp;
}

void MessageQueue::push(Message message)
{
	std::lock_guard<std::mutex> lock(messageMutex);
	messages.emplace(message);
}

bool MessageQueue::isEmpty()
{
	std::lock_guard<std::mutex> lock(messageMutex);
	return messages.empty();
}
