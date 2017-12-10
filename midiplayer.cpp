#include "midiplayer.hpp"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QAudioDeviceInfo>

#include <thread>

MIDIPlayer::MIDIPlayer(QWidget * parent) : QWidget(parent)
{
	playing = false;
	volume = 50 * VOL_MAX;
	newSong = true;

	auto layout = new QVBoxLayout;
	layout->addWidget(makeMidiParams());
	layout->addWidget(makePlayParams());
	setLayout(layout);

	connect(midiBtn, SIGNAL(clicked()), this, SLOT(midiFileBrowse()));
	connect(volumeSld, SIGNAL(valueChanged(int)), this, SLOT(onVolChange(int)));
	connect(playBtn, SIGNAL(clicked()), this, SLOT(onPlay()));
	connect(pauseBtn, SIGNAL(clicked()), this, SLOT(onPause()));
	connect(stopBtn, SIGNAL(clicked()), this, SLOT(onStop()));
	connect(muteBtn, SIGNAL(clicked()), this, SLOT(onMute()));
	processor = std::thread(&MIDIPlayer::processFiles, this);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	
	auto rates = info.supportedSampleRates();

	int sampleRate = rates.at(0);
	for (int i = 1; i < rates.size(); ++i) {
		if (rates.at(i) < sampleRate)
			sampleRate = rates.at(i);
	}

	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!info.isFormatSupported(format)) {
		qWarning()
			<< "Raw audio format not supported by backend, cannot play audio.";
	}
	else {
		audio = new QAudioOutput(format);
	}
}

MIDIPlayer::~MIDIPlayer()
{
	processor.join();
}

void MIDIPlayer::midiFileBrowse()
{
	QString midiFile = QFileDialog::getOpenFileName(this, "MIDI File",
		QDir::currentPath());

	if (!midiFile.isEmpty())
	{
		midiPath->setText(midiFile);
	}
}

QGroupBox* MIDIPlayer::makeMidiParams()
{
	auto layout = new QGridLayout;
	auto midiParams = new QGroupBox("MIDI Input");

	midiPath = new QLineEdit();
	midiPath->setObjectName("inputpath");
	midiBtn = new QPushButton("Browse");
	midiBtn->setObjectName("inputbrowse");
	
	layout->addWidget(midiPath, 0, 0);
	layout->addWidget(midiBtn, 0, 1);
	midiParams->setLayout(layout);
	return midiParams;
}

QGroupBox* MIDIPlayer::makePlayParams()
{
	auto layout = new QGridLayout;
	auto midiParams = new QGroupBox("Audio Controls");

	playBtn = new QPushButton("Play");
	playBtn->setObjectName("play");

	pauseBtn = new QPushButton("Pause");
	pauseBtn->setObjectName("pause");

	stopBtn = new QPushButton("Stop");
	stopBtn->setObjectName("stop");

	volumeSld = new QSlider(Qt::Horizontal);
	volumeSld->setObjectName("volume");
	volumeSld->setRange(0, 100);
	volumeSld->setSliderPosition(50);
	volumeSld->setTracking(false);

	muteBtn = new QPushButton("Mute");
	muteBtn->setObjectName("mute");
	
	layout->addWidget(playBtn, 0, 0);
	layout->addWidget(pauseBtn, 0, 1);
	layout->addWidget(stopBtn, 0, 2);
	layout->addWidget(new QLabel("Volume:"), 1, 0);
	layout->addWidget(volumeSld, 1, 1);
	layout->addWidget(muteBtn, 1, 2);

	midiParams->setLayout(layout);
	return midiParams;
}

void MIDIPlayer::onPlay()
{
	if (midiPath->text().isEmpty())
	{
		QMessageBox error;
		error.setObjectName("noinput");
		error.critical(this, "Error", "Must select a MIDI file");
		return;
	}
	playBtn->setEnabled(false);
	midiPath->setReadOnly(true);
	playing = true;
	pauseBtn->setEnabled(true);
	stopBtn->setEnabled(true);
	messages.push(Message::PLAY);
	qDebug() << "Hit play";
	//TODO send play message
}

void MIDIPlayer::onPause()
{
	playing = false;
	playBtn->setEnabled(true);
	pauseBtn->setEnabled(false);
	stopBtn->setEnabled(true);
	qDebug() << "Hit pause";
}

void MIDIPlayer::onStop()
{
	playBtn->setEnabled(true);
	pauseBtn->setEnabled(false);
	stopBtn->setEnabled(false);
	midiPath->setReadOnly(false);
	qDebug() << "Hit stop";
	//TODO send reset message
}

void MIDIPlayer::onMute()
{
	std::lock_guard<std::mutex> lock(volMutex);
	volume = volume == 0 ? volumeSld->sliderPosition() * VOL_MAX: 0;
	qDebug() << "Hit mute";
}

void MIDIPlayer::onVolChange(int volume)
{
	qDebug() << volume;
	//TODO implement
}

void MIDIPlayer::processFiles()
{
	//todo
	return;
}

void MIDIPlayer::handleNotify()
{
	//TODO implement;
	return;
}