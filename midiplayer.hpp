#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP

#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>

#include <thread>

// define class MIDIPlayer here
class MIDIPlayer : public QWidget {
Q_OBJECT

public:

	/**
	* Creates a new MIDIPlayer
	*/
	MIDIPlayer(QWidget * parent = nullptr);

	/**
	* Destroys the MIDIPlayer by closing its threads
	*/
	~MIDIPlayer();

	/**
	* Cannot be copied
	*/
	MIDIPlayer(const MIDIPlayer &) = delete;

	/**
	* Cannot be assigned
	*/
	MIDIPlayer & operator=(const MIDIPlayer &) = delete;

private slots:

	/**
	* Creates a QFileDialog that lets the user choose a midi file
	*/
	void midiFileBrowse();

	/**
	* Starts playing the MIDI file
	*/
	void onPlay();

	/**
	* Pauses playing of the MIDI file
	*/
	void onPause();

	/**
	* Stops play of the MIDI file
	*/
	void onStop();

	/**
	* Toggles whether sound is muted
	*/
	void onMute();

	/**
	* Changes the volume level based on the volume slider
	*/
	void onVolChange(int volume);

private:
	bool newTrack;

	std::thread processor;

	QLineEdit * midiPath;
	QPushButton * midiBtn;

	QPushButton * playBtn;
	QPushButton * pauseBtn;
	QPushButton * stopBtn;
	QPushButton * muteBtn;

	QSlider * volumeSld;

	/**
	* Creates a QGroupBox containing the MIDI parameters
	*/
	QGroupBox* makeMidiParams();

	/**
	* Creates a QGroupBox containing the play parameters
	*/
	QGroupBox* makePlayParams();

	/**
	* The file processing function that processes the signal
	*/
	void processFiles();
};
#endif
