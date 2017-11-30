#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include "midiplayer.hpp"
#include "message_queue.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();

  void testMessageQueue();
  
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

void TestMIDIPlayer::testMessageQueue() {
	MessageQueue queue;
	QCOMPARE(queue.isEmpty(), true);
	queue.push(Message(Message::PAUSE));
	QCOMPARE(queue.isEmpty(), false);
	Message temp = queue.pop();
	QCOMPARE(temp.isPause(), true);
	QCOMPARE(temp.isPlay(), false);
	QCOMPARE(temp.isStop(), false);
	QCOMPARE(temp.isExit(), false);
	QCOMPARE(queue.isEmpty(), true);
	QCOMPARE(queue.pop().isEmpty(), true);
}

QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

