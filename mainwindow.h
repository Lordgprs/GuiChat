#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Database.h"
#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(std::shared_ptr<Database> dbPtr = nullptr,
                      QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow *createClient(std::shared_ptr<Database> dbPtr = nullptr);
  static size_t kInstanceCount;

private slots:

  void on_messageLineEdit_returnPressed();
  void on_sendMessageButton_clicked();
  void on_privateMessageSendButton_clicked();
  void on_actionOpen_another_client_triggered();
  void on_actionClose_this_client_triggered();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Database> m_dbPtr;
};

#endif // MAINWINDOW_H
