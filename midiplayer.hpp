#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP

#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>

// define class MIDIPlayer here
class MIDIPlayer : public QWidget {
Q_OBJECT

public:

	MIDIPlayer(QWidget * parent = nullptr);

private slots:

	void midiFileBrowse();

	void onPlay();

	void onPause();

	void onStop();

	void onMute();

	void onVolChange(int volume);

private:

	QLineEdit * midiPath;
	QPushButton * midiBtn;

	QPushButton * playBtn;
	QPushButton * pauseBtn;
	QPushButton * stopBtn;
	QPushButton * muteBtn;

	QSlider * volumeSld;

	QGroupBox* makeMidiParams();

	QGroupBox* makePlayParams();

	void processFiles();

};
#endif
