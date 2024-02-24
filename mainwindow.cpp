#include "mainwindow.h"
#include "Database.h"
#include "startscreen.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTimer>
#include <QVBoxLayout>

size_t MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(size_t userId, QString userName,
                       std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), m_userId{userId}, m_userName{userName} {
  ui->setupUi(this);
  ++kInstanceCount;
  if (dbPtr) {
    m_dbPtr = dbPtr;
  } else {
    m_dbPtr = std::make_shared<Database>();
  }
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
  auto w{new MainWindow(s.userId(), s.userName(), dbPtr)};
  w->setAttribute(Qt::WA_DeleteOnClose);
  return w;
}

void MainWindow::on_messageLineEdit_returnPressed() {
  on_sendMessageButton_clicked();
}

void MainWindow::on_sendMessageButton_clicked() {
  m_dbPtr->addChatMessage(m_userName.toStdString(),
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

  auto userList{m_dbPtr->getUserList()};
  for (const auto &user : userList) {
    userListWgt->addItem(QString::fromStdString(user));
  }
  userListWgt->setCurrentRow(0);

  auto result{dial.exec()};
  if (result == QDialog::Accepted && userListWgt->currentItem()) {
    m_dbPtr->addPrivateMessage(m_userName.toStdString(),
                               userListWgt->currentItem()->text().toStdString(),
                               ui->messageLineEdit->text().toStdString());
  }
}

void MainWindow::updateChats() {
  auto chatMessages = m_dbPtr->getChatMessages();
  QString chat;
  for (const auto &msg : chatMessages) {
    chat.append(QString::fromStdString(msg) + '\n');
  }
  ui->commonChatBrowser->setText(chat);

  chat.clear();
  auto privateMessages = m_dbPtr->getPrivateMessage(m_userId);
  for (const auto &msg : privateMessages) {
    if (msg.getSender() != m_userName.toStdString() &&
        msg.getDest() != m_userId) {
      continue;
    }
    QString prefix;
    if (m_userName.toStdString() == msg.getSender() &&
        m_userId == msg.getDest()) {
      prefix = tr("self") + ": ";
    } else if (m_userName.toStdString() == msg.getSender()) {
      prefix = tr("you to") + ": " +
               QString(" <%1>: ").arg(
                   QString::fromStdString(m_dbPtr->getUserName(msg.getDest())));
    } else {
      prefix = QString("<%1>").arg(QString::fromStdString(
                   m_dbPtr->getUserName(msg.getDest()))) +
               tr(" to you: ");
    }
    chat.append(prefix + QString::fromStdString(msg.getText() + '\n'));
  }
  ui->privateChatBrowser->setText(chat);
}

void MainWindow::on_actionNew_instance_triggered() {
  auto w{createClient(m_dbPtr)};
  if (w) {
    w->show();
  }
}

void MainWindow::on_actionClose_triggered() { this->close(); }
