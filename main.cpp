#include <QApplication>

#include "midiplayer.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MIDIPlayer player;
  player.show();
  return app.exec();
}
