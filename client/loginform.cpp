#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) : QWidget(parent), ui(new Ui::LoginForm) {
  ui->setupUi(this);
  auto button_ok{ui->buttonBox->button(QDialogButtonBox::Ok)};
  auto button_cancel{ui->buttonBox->button(QDialogButtonBox::Cancel)};

  button_cancel->setAutoDefault(false);
  button_cancel->setDefault(false);
  button_ok->setAutoDefault(true);
  button_ok->setDefault(true);
}

LoginForm::~LoginForm() { delete ui; }

void LoginForm::setDatabase(std::shared_ptr<Database> dbPtr) {
  m_dbPtr = dbPtr;
}

void LoginForm::on_buttonBox_accepted() {
  auto userId{m_dbPtr->checkPassword(ui->loginEdit->text().toStdString(),
                                     ui->passwordEdit->text().toStdString())};
  if (userId == -1) {
    QMessageBox::critical(this, tr("Error"), tr("Error: password is wrong"));
    return;
  }
  emit accepted(userId, ui->loginEdit->text());
}

void LoginForm::on_buttonBox_rejected() { emit rejected(); }

void LoginForm::on_registrationPushButton_clicked() {
  emit registerRequested();
}
