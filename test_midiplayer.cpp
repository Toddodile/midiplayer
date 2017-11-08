#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include "midiplayer.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();

  // TODO: add your tests
  
private:

  MIDIPlayer widget;
};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
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

QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

