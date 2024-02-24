#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Database.h"
#include <QMainWindow>
#include <chrono>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(size_t userId, QString userName,
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
  std::shared_ptr<Database> m_dbPtr;
  size_t m_userId;
  QString m_userName;
  const std::chrono::milliseconds TIMEOUT{50ms};
};

#endif // MAINWINDOW_H
