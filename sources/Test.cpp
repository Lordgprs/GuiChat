#include "CommandLineInterface.h"
#include "mainwindow.h"
#include "qmessagebox.h"
#include <QApplication>
#include <QTranslator>
#include <iostream>
// using namespace std;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator myappTranslator;
  myappTranslator.load("translations/my_ru.qm");
  a.installTranslator(&myappTranslator);

  QTranslator qtTranslator;
  qtTranslator.load("translations/my_ru.qm");
  a.installTranslator(&qtTranslator);

  //	cout << "Usage:" << endl;
  //	cout << "!login[username][password] - log in" << endl;
  //	cout << "!logout - log out" << endl;
  //	cout << "!register[username][password] - register new user" << endl;
  //	cout << "!exit for exit" << endl;
  //	cout << "!pm[(username)] message - write private message" << endl;
  //	cout << "!getPM - receive private messages" << endl;
  //  CommandLineInterface cli{};
  //  cout << "chat ver. 0.1beta" << endl;
  //  while (!cli->shouldExit())
  //  {
  //    cli->parseCommand();
  //  }

  //  QTranslator myappTranslator;
  //  myappTranslator.load("translations/my_ru.qm");
  //  a.installTranslator(&myappTranslator);

  //  QTranslator defaultTranslator;
  //  defaultTranslator.load("translations/qt_ru.qm");
  //  a.installTranslator(&defaultTranslator);
  MainWindow *w{nullptr};
  try {
    w = MainWindow::createClient();
  } catch (const std::invalid_argument &e) {
    QMessageBox::critical(nullptr, QObject::tr("Fatal error"),
                          QObject::tr(e.what()));
  }
  if (w) {
    w->show();
  } else {
    return EXIT_SUCCESS;
  }

  return a.exec();
}
