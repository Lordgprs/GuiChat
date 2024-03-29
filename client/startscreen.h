#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Database.h"
#include "Settings.h"
#include <QDialog>
#include <QSettings>
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

  std::shared_ptr<Database> getDatabase() const;

public slots:
  void onLoggedIn(uint userId, QString userName);
  void onRejectRequested();

private:
  Ui::StartScreen *ui;
  int _userId;
  QString _userName;
  Settings::ConfigurationParameters _params;
  std::shared_ptr<Database> _dbPtr;
  std::shared_ptr<QSettings> _settings;
  const char *const CONFIG_FILE_NAME{"chat.conf"};
};

#endif // STARTSCREEN_H
