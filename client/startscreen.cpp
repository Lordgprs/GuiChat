#include "startscreen.h"
#include "ui_startscreen.h"
#include <QMessageBox>
#include <iostream>

StartScreen::StartScreen(std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QDialog(parent), ui(new Ui::StartScreen) {
  ui->setupUi(this);
  _settings = Settings::loadSettings(CONFIG_FILE_NAME, _params);
  if (dbPtr) {
    _dbPtr = dbPtr;
  } else {
    _dbPtr = std::make_shared<Database>(_params.db_name, _params.db_host,
                                        _params.db_user, _params.db_password);
  }
  ui->loginWidget->setDatabase(_dbPtr);
  ui->registerWidget->setDatabase(_dbPtr);

  connect(ui->loginWidget, &LoginForm::registerRequested, this,
          &StartScreen::setRegistrationForm);
  connect(ui->loginWidget, &LoginForm::accepted, this,
          &StartScreen::onLoggedIn);
  connect(ui->loginWidget, &LoginForm::rejected, this,
          &StartScreen::onRejectRequested);
  connect(ui->registerWidget, &RegistrationForm::loginRequested, this,
          &StartScreen::setLoginForm);
  connect(ui->registerWidget, &RegistrationForm::accepted, this,
          &StartScreen::onLoggedIn);
  connect(ui->registerWidget, &RegistrationForm::rejected, this,
          &StartScreen::onRejectRequested);
}

StartScreen::~StartScreen() { delete ui; }

void StartScreen::setLoginForm() { ui->stackedWidget->setCurrentIndex(0); }

void StartScreen::setRegistrationForm() {
  ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::onLoggedIn(uint userId, QString userName) {
  _userId = userId;
  _userName = userName;
  accept();
}

void StartScreen::onRejectRequested() { reject(); }

std::shared_ptr<Database> StartScreen::getDatabase() const { return _dbPtr; }

const QString &StartScreen::userName() const { return _userName; }

int StartScreen::userId() const { return _userId; }
