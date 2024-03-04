#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Database.h"
#include "Settings.h"
#include <QMainWindow>
#include <chrono>

using namespace std::chrono_literals;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Database> _dbPtr;
  std::shared_ptr<QSettings> _settings;
  const std::chrono::milliseconds TIMEOUT{50ms};
  const char *const CONFIG_FILE_NAME{"chat.conf"};
  Settings::ConfigurationParameters _params;
};
#endif // MAINWINDOW_H
