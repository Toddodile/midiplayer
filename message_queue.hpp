#include "track.hpp"

#include <queue>
#include <mutex>

class Message {
public:
	enum TYPE { PLAY, PAUSE, STOP, EXIT };

	Message(Track newTrack) {
		messageType = PLAY;
		track = newTrack;
	}

	Message(TYPE message) {
		messageType = message;
	}

	bool isPlay() {
		return messageType == PLAY;
	}

	bool isPause() {
		return messageType == PAUSE;
	}

	bool isStop() {
		return messageType == STOP;
	}

	bool isExit() {
		return messageType == EXIT;
	}

	Track getTrack() { 
		return track;
	}

private:
	Track track;
	TYPE messageType;
};

class MessageQueue {
public:
	MessageQueue();
	
	Message pop();

	void push(Message message);

	bool isEmpty();

private:

	std::queue<Message> messages;

	mutable std::mutex messageMutex;
};