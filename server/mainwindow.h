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

private slots:
  void on_btnRefresh_clicked();

  void on_lstMessages_itemSelectionChanged();

  void on_btnRemoveMessage_clicked();

  void on_lstUsers_itemSelectionChanged();

  void on_btnDisableUser_clicked();

  void on_btnRemoveUser_clicked();

private:
  void refreshMessageList();
  void refreshUserList();

  Ui::MainWindow *ui;
  std::shared_ptr<Database> _dbPtr;
  std::shared_ptr<QSettings> _settings;
  const std::chrono::milliseconds TIMEOUT{50ms};
  const int UPDATE_LISTS_INTERVAL{1000};
  const int UPDATE_LISTS_IMMIDIATELY{0};
  const char *const CONFIG_FILE_NAME{"chat.conf"};
  Settings::ConfigurationParameters _params;
  std::map<int, int> _messages_listitems;
  std::vector<Message> _messages;
  std::map<int, int> _users_listitems;
  std::vector<std::string> _users;
};
#endif // MAINWINDOW_H
