//#include "CommandLineInterface.h"
#include "DbException.h"
#include "mainwindow.h"
#include "qmessagebox.h"
#include <QApplication>
#include <QTranslator>
#include <iostream>
// using namespace std;

int main(int argc, char *argv[]) {
  int result{EXIT_SUCCESS};
  QApplication a(argc, argv);

  QTranslator myappTranslator;
  myappTranslator.load("translations/my_ru.qm");
  a.installTranslator(&myappTranslator);

  QTranslator qtTranslator;
  qtTranslator.load("translations/my_ru.qm");
  a.installTranslator(&qtTranslator);

  MainWindow *w{nullptr};
  try {
    w = MainWindow::createClient();
  } catch (const std::invalid_argument &e) {
    QMessageBox::critical(nullptr, QObject::tr("Fatal error"),
                          QObject::tr(e.what()));
  } catch (const DbException &e) {
    QMessageBox::critical(nullptr, QObject::tr("Database error"),
                          QObject::tr(e.what()));
  } catch (std::exception &e) {
    QMessageBox::critical(nullptr, QObject::tr("Unknown error"),
                          QObject::tr(e.what()));
  }

  if (w) {
    w->show();
  } else {
    return result;
  }

  try {
    result = a.exec();
  } catch (const DbException &e) {
    QMessageBox::critical(nullptr, QObject::tr("Database error"),
                          QObject::tr(e.what()));
  }
  //  catch (std::exception &e) {
  //    QMessageBox::critical(nullptr, QObject::tr("Unknown error"),
  //                          QObject::tr(e.what()));
  //  }

  return result;
}
