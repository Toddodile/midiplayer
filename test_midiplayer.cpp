#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include "test_config.hpp"
#include "midiplayer.hpp"
#include "message_queue.hpp"
#include "shared_double_buffer.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();

  void functionalTests();

  void testMessageQueue();

  void testDoubleBuffer();
  
private:

  MIDIPlayer widget;
};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
	auto w = widget.findChild<QPushButton *>("inputbrowse");
	QVERIFY2(w, "Could not find a file browse button");
  }

  {
    auto w = widget.findChild<QPushButton *>("play");
    QVERIFY2(w, "Could not find play button");
  }

  {
    auto w = widget.findChild<QPushButton *>("pause");
    QVERIFY2(w, "Could not find pause button");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("stop");
    QVERIFY2(w, "Could not find stop button");
  }

  {
    auto w = widget.findChild<QSlider *>("volume");
    QVERIFY2(w, "Could not find volume slider");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("mute");
    QVERIFY2(w, "Could not find mute button");
  }
}

void TestMIDIPlayer::functionalTests() {
	widget.show();
	auto input = widget.findChild<QLineEdit *>("inputpath");
	auto play = widget.findChild<QPushButton *>("play");
	auto pause = widget.findChild<QPushButton *>("pause");
	auto stop = widget.findChild<QPushButton *>("stop");
	auto mute = widget.findChild<QPushButton *>("mute");
	auto volume = widget.findChild<QSlider *>("volume");

	QVERIFY(play->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(!stop->isEnabled());
	QVERIFY(mute->isEnabled());
	QCOMPARE(volume->sliderPosition(), 50);
	QCOMPARE(widget.getState(), MIDIPlayer::STOPPED);

	QTest::mouseClick(mute, Qt::LeftButton, Qt::NoModifier, QPoint(), 125);
	QCOMPARE(volume->sliderPosition(), 0);
	volume->setSliderPosition(50);

	input->setText(TEST_FILE);
	QTest::mouseClick(play, Qt::LeftButton, Qt::NoModifier, QPoint(), 125);
	QVERIFY(input->isReadOnly());
	QVERIFY(!play->isEnabled());
	QVERIFY(pause->isEnabled());
	QVERIFY(stop->isEnabled());
	QCOMPARE(widget.getState(), MIDIPlayer::PLAYING);

	QTest::mouseClick(pause, Qt::LeftButton, Qt::NoModifier, QPoint(), 125);
	QVERIFY(input->isReadOnly());
	QVERIFY(play->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(stop->isEnabled());
	QCOMPARE(widget.getState(), MIDIPlayer::PAUSED);

	QTest::mouseClick(play, Qt::LeftButton, Qt::NoModifier, QPoint(), 125);
	QVERIFY(input->isReadOnly());
	QVERIFY(!play->isEnabled());
	QVERIFY(pause->isEnabled());
	QVERIFY(stop->isEnabled());
	QCOMPARE(widget.getState(), MIDIPlayer::PLAYING);

	QTest::mouseClick(stop, Qt::LeftButton, Qt::NoModifier, QPoint(), 125);
	QVERIFY(!input->isReadOnly());
	QVERIFY(play->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(!stop->isEnabled());
	QCOMPARE(widget.getState(), MIDIPlayer::STOPPED);
}

void TestMIDIPlayer::testMessageQueue() {
	MessageQueue queue;
	QCOMPARE(queue.isEmpty(), true);
	queue.push(Message(Message::STOP));
	QCOMPARE(queue.isEmpty(), false);
	Message temp = queue.pop();
	QCOMPARE(temp.isPlay(), false);
	QCOMPARE(temp.isStop(), true);
	QCOMPARE(temp.isExit(), false);
	QCOMPARE(queue.isEmpty(), true);
	QCOMPARE(queue.pop().isEmpty(), true);
}

void TestMIDIPlayer::testDoubleBuffer() {
	SharedDoubleBuffer buffer;
	QCOMPARE(buffer.getMaxSize(), (size_t)8192);
	QVERIFY(buffer.isEmpty());
	QCOMPARE(buffer.pop(), (int16_t)0);
	QVERIFY(buffer.isEmpty());
	for (int16_t i = 1; i <= 8192; i++)
	{
		QVERIFY(buffer.tryPush(i));
	}
	QVERIFY(!buffer.tryPush(10));
	QCOMPARE(buffer.pop(), (int16_t)1);
	QVERIFY(buffer.tryPush(8193));
	for (int16_t i = 2; i <= 8193; i++)
	{
		QCOMPARE(buffer.pop(), i);
	}
	QVERIFY(buffer.isEmpty());
	buffer.tryPush(1);
	buffer.tryPush(10);
	QVERIFY(!buffer.isEmpty());
	buffer.clear();
	QVERIFY(buffer.isEmpty());
	QCOMPARE(buffer.pop(), (int16_t)0);
}

QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

