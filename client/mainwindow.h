#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Database.h"
#include <QMainWindow>
#include <QSettings>
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(int userId, QString userName,
                      std::shared_ptr<Database> dbPtr = nullptr,
                      QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow *createClient(std::shared_ptr<Database> dbPtr = nullptr);
  static size_t kInstanceCount;

private slots:
  void on_messageLineEdit_returnPressed();
  void on_sendMessageButton_clicked();
  void on_privateMessageSendButton_clicked();
  void updateChats();
  void on_actionNew_instance_triggered();
  void on_actionClose_triggered();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Database> _dbPtr;
  int _userId;
  QString _userName;
  std::shared_ptr<QSettings> _settings;
  const std::chrono::milliseconds TIMEOUT{50ms};
};

#endif // MAINWINDOW_H
