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

	std::thread processor(&processFiles);

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
	qDebug() << "Hit play";
	//TODO implement
}

void MIDIPlayer::onPause()
{
	qDebug() << "Hit pause";
	//TODO implement
}

void MIDIPlayer::onStop()
{
	qDebug() << "Hit stop";
	//TODO implement
}

void MIDIPlayer::onMute()
{
	qDebug() << "Hit mute";
	//TODO implement
}

void MIDIPlayer::onVolChange(int volume)
{
	qDebug() << volume;
	//TODO implement
}

void MIDIPlayer::processFiles()
{
	//TODO implement
	return;
}