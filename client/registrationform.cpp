#include "registrationform.h"
#include "ui_registrationform.h"
#include <QMessageBox>

RegistrationForm::RegistrationForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegistrationForm) {
  ui->setupUi(this);
  auto button_ok{ui->buttonBox->button(QDialogButtonBox::Ok)};
  auto button_cancel{ui->buttonBox->button(QDialogButtonBox::Cancel)};

  button_cancel->setAutoDefault(false);
  button_cancel->setDefault(false);
  button_ok->setAutoDefault(true);
  button_ok->setDefault(true);
}

RegistrationForm::~RegistrationForm() { delete ui; }

void RegistrationForm::setDatabase(std::shared_ptr<Database> dbPtr) {
  m_dbPtr = dbPtr;
}

void RegistrationForm::on_buttonBox_accepted() {
  if (ui->passwordEdit->text() != ui->passwordConfirmEdit->text()) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Error: passwords don't match"));
    return;
  }
  auto userId{m_dbPtr->addUser(ui->loginEdit->text().toStdString(),
                               ui->passwordEdit->text().toStdString())};
  switch (userId) {
  case -1:
    QMessageBox::critical(this, tr("Error"), tr("Incorrect login"));
    return;
  case -2:
    QMessageBox::critical(this, tr("Error"), tr("Login already exists;"));
    return;
  default:
    emit accepted(userId, ui->loginEdit->text());
  }
}

void RegistrationForm::on_buttonBox_rejected() { emit rejected(); }

void RegistrationForm::on_loginButton_clicked() { emit loginRequested(); }
