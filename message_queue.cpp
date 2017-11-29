#include "message_queue.hpp"
#include "track.hpp"

#include <queue>
#include <mutex>

class Message {
public:
	enum TYPE { PLAY, PAUSE, STOP, EXIT };
	Message(Track track);

	Message(TYPE message);

	bool isPlay();

	bool isPause();

	bool isStop();

	bool isExit();

	Track getTrack();
};

class MessageQueue {
public:
	MessageQueue();


private:



	mutable std::mutex myMutex;
};