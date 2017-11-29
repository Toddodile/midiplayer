#include "message_queue.hpp"

#include <queue>
#include <mutex>

class MessageQueue {
public:
	MessageQueue();
private:
	mutable std::mutex myMutex;
};