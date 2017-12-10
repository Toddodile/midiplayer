#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP
#include "track.hpp"

#include <queue>
#include <mutex>

class Message {
public:
	enum TYPE { PLAY, STOP, EXIT, EMPTY };

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

	bool isStop() {
		return messageType == STOP;
	}

	bool isExit() {
		return messageType == EXIT;
	}

	bool isEmpty() {
		return messageType == EMPTY;
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

	/**
	* Cannot be copied
	*/
	MessageQueue(const MessageQueue &) = delete;

	/**
	* Cannot be assigned
	*/
	MessageQueue & operator=(const MessageQueue &) = delete;
	
	Message pop();

	void push(Message message);

	bool isEmpty();

private:

	std::queue<Message> messages;

	mutable std::mutex messageMutex;
};

#endif
