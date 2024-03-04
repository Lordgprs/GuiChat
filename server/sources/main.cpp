#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <variant>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  std::variant<MainWindow, std::monostate> w;
  try {
    w.emplace<MainWindow>();
  } catch (const std::invalid_argument &e) {
    QMessageBox::critical(nullptr, QObject::tr("Fatal error"),
                          QObject::tr(e.what()));
  }
  try {
    std::get<MainWindow>(w).show();
  } catch (const std::bad_variant_access &e) {
    QMessageBox::critical(nullptr, QObject::tr("Fatal error"),
                          QObject::tr("can not create main window"));
  }

  return a.exec();
}
