#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  _settings = Settings::loadSettings(CONFIG_FILE_NAME, _params);
  _dbPtr = std::make_shared<Database>(_params.db_name, _params.db_host,
                                      _params.db_user, _params.db_password);
  ui->setupUi(this);

  auto timer{new QTimer{this}};
  timer->setInterval(UPDATE_LISTS_INTERVAL);
  connect(timer, &QTimer::timeout, this, [&]() {
    refreshMessageList();
    refreshUserList();
  });
  timer->start(UPDATE_LISTS_IMMIDIATELY);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::refreshMessageList() {
  auto messages{_dbPtr->getAllMessages()};
  if (_messages == messages) {
    return;
  }

  int list_item_index{0};
  _messages = messages;
  ui->lstMessages->clear();
  _messages_listitems.clear();
  for (const auto &message : messages) {
    QString dest{
        message.getDest() == -1
            ? tr("all")
            : QString::fromStdString(_dbPtr->getUserName(message.getDest()))};
    QString line{QString::fromStdString(message.getTime()) + " " +
                 QString::fromStdString(message.getSender()) + " " + tr("to") +
                 " " + dest + ": " + QString::fromStdString(message.getText())};
    ui->lstMessages->addItem(line);
    _messages_listitems.insert({list_item_index++, message.getID()});
  }
}

void MainWindow::refreshUserList() {
  auto users{_dbPtr->getUserList()};
  if (users == _users) {
    return;
  }

  _users = users;
  int list_item_index{0};
  ui->lstUsers->clear();
  _users_listitems.clear();
  for (const auto &user : users) {
    ui->lstUsers->addItem(QString::fromStdString(user));
    _users_listitems.insert(
        {list_item_index++, _dbPtr->searchUserByName(user)});
  }
}

void MainWindow::on_btnRefresh_clicked() {
  refreshMessageList();
  refreshUserList();
}

void MainWindow::on_lstMessages_itemSelectionChanged() {
  bool hasSelection{ui->lstMessages->currentItem()->isSelected()};
  if (hasSelection) {
    ui->btnRemoveMessage->setDisabled(false);
  } else {
    ui->btnRemoveMessage->setDisabled(true);
  }
}

void MainWindow::on_lstUsers_itemSelectionChanged() {
  bool hasSelection{ui->lstUsers->currentItem()->isSelected()};
  if (hasSelection) {
    ui->btnDisableUser->setDisabled(false);
    ui->btnRemoveUser->setDisabled(false);
  } else {
    ui->btnDisableUser->setDisabled(true);
    ui->btnRemoveUser->setDisabled(true);
  }
}

void MainWindow::on_btnRemoveMessage_clicked() {
  auto answer{
      QMessageBox::question(this, tr("Remove message"), tr("Are you sure?"))};
  if (answer == QMessageBox::Yes) {
    _dbPtr->removeMessage(
        _messages_listitems[ui->lstMessages->currentIndex().row()]);
    refreshMessageList();
  }
  //_dbPtr->removeMessage(ui->lstMessages->currentIndex().row());
}

void MainWindow::on_btnDisableUser_clicked() {
  auto answer{
      QMessageBox::question(this, tr("Disable user"), tr("Are you sure?"))};
  if (answer == QMessageBox::Yes) {
    _dbPtr->disableUser(_users_listitems[ui->lstUsers->currentIndex().row()]);
    refreshUserList();
  }
}

void MainWindow::on_btnRemoveUser_clicked() {
  auto answer{
      QMessageBox::question(this, tr("Remove user"), tr("Are you sure?"))};
  if (answer == QMessageBox::Yes) {
    _dbPtr->removeUser(_users_listitems[ui->lstUsers->currentIndex().row()]);
    refreshUserList();
  }
}
