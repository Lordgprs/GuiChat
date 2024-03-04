#include "mainwindow.h"
#include "Database.h"
#include "startscreen.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <iostream>

size_t MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(int userId, QString userName,
                       std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), _dbPtr{dbPtr}, _userId{userId}, _userName{
                                                                  userName} {
  if (!dbPtr) {
    throw std::runtime_error{"database is not valid"};
  }
  ui->setupUi(this);
  ui->userNameLabel->setText(_userName);
  setWindowTitle("Chat: " + _userName);
  ++kInstanceCount;
  auto timer{new QTimer{this}};
  connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
  timer->start(MainWindow::TIMEOUT);
}

MainWindow::~MainWindow() {
  delete ui;
  if (--kInstanceCount == 0) {
    qApp->exit(EXIT_SUCCESS);
  }
}

MainWindow *MainWindow::createClient(std::shared_ptr<Database> dbPtr) {
  StartScreen s{dbPtr};
  auto result{s.exec()};
  if (result == QDialog::Rejected) {
    return nullptr;
  }
  dbPtr = s.getDatabase();
  auto w{new MainWindow(s.userId(), s.userName(), dbPtr)};
  w->setAttribute(Qt::WA_DeleteOnClose);
  return w;
}

void MainWindow::on_messageLineEdit_returnPressed() {
  on_sendMessageButton_clicked();
}

void MainWindow::on_sendMessageButton_clicked() {
  _dbPtr->addChatMessage(_userName.toStdString(),
                         ui->messageLineEdit->text().toStdString());
}

void MainWindow::on_privateMessageSendButton_clicked() {
  QDialog dial{this};
  dial.setModal(true);
  auto *l{new QVBoxLayout};
  dial.setLayout(l);
  auto userListWgt{new QListWidget{&dial}};
  l->addWidget(userListWgt);
  auto buttonBox{
      new QDialogButtonBox{QDialogButtonBox::Ok | QDialogButtonBox::Cancel}};
  l->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

  auto userList{_dbPtr->getUserList()};
  for (const auto &user : userList) {
    userListWgt->addItem(QString::fromStdString(user));
  }
  userListWgt->setCurrentRow(0);

  auto result{dial.exec()};
  if (result == QDialog::Accepted && userListWgt->currentItem()) {
    _dbPtr->addPrivateMessage(_userName.toStdString(),
                              userListWgt->currentItem()->text().toStdString(),
                              ui->messageLineEdit->text().toStdString());
  }
}

void MainWindow::updateChats() {
  auto chatMessages = _dbPtr->getChatMessages();
  QString chat;
  for (const auto &msg : chatMessages) {
    chat.append(QString::fromStdString(msg) + '\n');
  }
  if (ui->commonChatBrowser->toPlainText() != chat) {
    ui->commonChatBrowser->setText(chat);
  }

  chat.clear();
  auto privateMessages = _dbPtr->getPrivateMessages(_userId);
  for (const auto &msg : privateMessages) {
    if (msg.getSender() != _userName.toStdString() &&
        msg.getDest() != _userId) {
      continue;
    }
    QString timestamp{QString::fromStdString(msg.getTime()).append(" ")};
    QString prefix;

    if (_userName.toStdString() == msg.getSender() &&
        _userId == msg.getDest()) {
      prefix = tr("self") + ": ";
    } else if (_userName.toStdString() == msg.getSender()) {
      prefix = tr("you to") + QString(" <%1>: ").arg(QString::fromStdString(
                                  _dbPtr->getUserName(msg.getDest())));
    } else {
      prefix = QString("<%1>").arg(
                   QString::fromStdString(_dbPtr->getUserName(msg.getDest()))) +
               tr(" to you: ");
    }
    chat.append(timestamp + prefix +
                QString::fromStdString(msg.getText() + '\n'));
  }
  if (ui->privateChatBrowser->toPlainText() != chat)
    ui->privateChatBrowser->setText(chat);
}

void MainWindow::on_actionNew_instance_triggered() {
  auto w{createClient(_dbPtr)};
  if (w) {
    w->show();
  }
}

void MainWindow::on_actionClose_triggered() { this->close(); }
