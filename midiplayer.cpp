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
#include <vector>
#include "midi.hpp"
#include "instrument.hpp"

MIDIPlayer::MIDIPlayer(QWidget * parent) : QWidget(parent)
{
	playing = false;
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

	connect(audio, SIGNAL(notify()), this, SLOT(handleNotify()));

	bufferSize = audio->bufferSize();
	bufferSize = bufferSize > 0 ? bufferSize : 1024;

	audio->setNotifyInterval(25);
	
	device = audio->start();

	processor = std::thread(&MIDIPlayer::processFiles, this, sampleRate);
	handleNotify();
}

MIDIPlayer::~MIDIPlayer()
{
	messages.push(Message(Message::EXIT));
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
	pauseBtn->setEnabled(false);

	stopBtn = new QPushButton("Stop");
	stopBtn->setObjectName("stop");
	stopBtn->setEnabled(false);

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
	std::lock_guard<std::mutex> songLock(songMutex);
	std::lock_guard<std::mutex> playLock(playingMutex);
	if (newSong)
	{
		TrackListType tracks = readMIDIFromFile(midiPath->text().toStdString());
		if (tracks.size() == 0)
		{
			QMessageBox error;
			error.setObjectName("badinput");
			error.critical(this, "Error", "Invlad MIDI file");
			return;
		}
		messages.push(Message(tracks.front()));
	}
	playing = true;
	qDebug() << "Hit play";
}

void MIDIPlayer::onPause()
{
	playing = false;
	playBtn->setEnabled(true);
	pauseBtn->setEnabled(false);
	stopBtn->setEnabled(true);
	std::lock_guard<std::mutex> lock(playingMutex);
	playing = false;
}

void MIDIPlayer::onStop()
{
	playBtn->setEnabled(true);
	pauseBtn->setEnabled(false);
	stopBtn->setEnabled(false);
	midiPath->setReadOnly(false);
	std::lock_guard<std::mutex> lock(playingMutex);
	playing = false;
	messages.push(Message(Message::STOP));
	std::lock_guard<std::mutex> songLock(songMutex);
	newSong = true;
}

void MIDIPlayer::onMute()
{
	volumeSld->setSliderPosition(0);
	audio->setVolume(0);
}

void MIDIPlayer::onVolChange(int vol)
{
	audio->setVolume(vol / VOL_MAX);
}

void MIDIPlayer::processFiles(double sampleRate)
{
	bool initialized = false;
	bool lastSamplePushed = true;
	Track track;
	DefaultInstrument instrument(track);
	double sample;
	while (true)
	{
		if (!messages.isEmpty())
		{
			Message message = messages.pop();
			if (message.isExit())
			{
				return;
			}
			if (message.isPlay())
			{
				instrument.reset(message.getTrack());
				initialized = true;
			}
			else if (message.isStop())
			{
				instrument.reset(track);
				initialized = false;
				lastSamplePushed = true;
			}
		}
		if (initialized && !instrument.halted())
		{
			if (lastSamplePushed)
			{
				sample = instrument.sample(1.0 / sampleRate);
			}
			lastSamplePushed = dataBuffer.tryPush((int16_t)sample);
		}
	}
	return;
}

void MIDIPlayer::handleNotify()
{
	int bytesFree = audio->bytesFree();
	int bytesToWrite = (int)(bytesFree > bufferSize ? bufferSize : bytesFree);
	int samplesToWrite = bytesToWrite >> 1;
	bytesToWrite = 2 * samplesToWrite;
	std::vector<char> buff;
	buff.reserve(bytesToWrite);
	int16_t *buffAsInt = (int16_t *)buff.data();
	for (int i = 0; i < samplesToWrite; i++)
	{
		buffAsInt[i] = dataBuffer.pop();
	}
	device->write(buff.data(), bytesToWrite);
}