#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Database.h"
#include <QDialog>
#include <memory>

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog {
  Q_OBJECT

public:
  explicit StartScreen(std::shared_ptr<Database> dbPtr = nullptr,
                       QWidget *parent = nullptr);
  ~StartScreen();
  void setLoginForm();
  void setRegistrationForm();

  int userId() const;

  const QString &userName() const;

public slots:
  void onLoggedIn(uint userId, QString userName);
  void onRejectRequested();

private:
  Ui::StartScreen *ui;
  int m_userId;
  QString m_userName;
  std::shared_ptr<Database> m_dbPtr;
};

#endif // STARTSCREEN_H