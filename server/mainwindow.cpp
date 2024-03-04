#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  _settings = Settings::loadSettings(CONFIG_FILE_NAME, _params);
  _dbPtr = std::make_shared<Database>(_params.db_name, _params.db_host,
                                      _params.db_user, _params.db_password);
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }
