#include "mainwindow.h"
#include "Database.h"
#include "startscreen.h"
#include "ui_mainwindow.h"

size_t MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ++kInstanceCount;
  if (dbPtr) {
    m_dbPtr = dbPtr;
  } else {
    m_dbPtr = std::make_shared<Database>();
  }
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
  auto w{new MainWindow(dbPtr)};
  w->setAttribute(Qt::WA_DeleteOnClose);
  return w;
}

void MainWindow::on_messageLineEdit_returnPressed() {
  on_sendMessageButton_clicked();
}

void MainWindow::on_sendMessageButton_clicked() {}

void MainWindow::on_privateMessageSendButton_clicked() {}

void MainWindow::on_actionOpen_another_client_triggered() {
  auto w{createClient(m_dbPtr)};
  if (w) {
    w->show();
  }
}

void MainWindow::on_actionClose_this_client_triggered() {
  QApplication::exit(EXIT_SUCCESS);
}
