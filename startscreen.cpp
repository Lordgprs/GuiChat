#include "startscreen.h"
#include "ui_startscreen.h"
#include <QMessageBox>
#include <iostream>

StartScreen::StartScreen(std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QDialog(parent), ui(new Ui::StartScreen) {
  ui->setupUi(this);
  loadSettings();
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

void StartScreen::loadSettings() {
  _settings =
      std::make_shared<QSettings>(CONFIG_FILE_NAME, QSettings::IniFormat);
  if (_settings->status() != QSettings::NoError) {
    throw std::invalid_argument{"error while opening configuration file"};
  }
  //  _settings->beginGroup("General");
  auto db_name{_settings->value("DBName")};
  if (!db_name.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database name, check configuration file"};
  }
  _params.db_name = db_name.value<QString>();
  auto db_host{_settings->value("DBHost")};
  if (!db_host.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database server hostname, check configuration file"};
  }
  _params.db_host = db_host.value<QString>();
  auto db_user{_settings->value("DBUser")};
  if (!db_user.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database user name, check configuration file"};
  }
  _params.db_user = db_user.value<QString>();
  auto db_password{_settings->value("DBPassword")};
  if (!db_password.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database user password, check configuration file"};
  }
  _params.db_password = db_password.value<QString>();
  //  _settings->setValue("DBName", "chat");
  //  _settings->setValue("DBHost", "127.0.0.1");
  //  _settings->setValue("DBUser", "chat");
  //  _settings->setValue("DBPassword", "12345");
  //  _settings->endGroup();
  //  _settings.reset();
  QMessageBox::information(nullptr, "DB name", _params.db_name);
}
