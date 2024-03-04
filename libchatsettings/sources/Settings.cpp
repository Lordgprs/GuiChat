#include "Settings.h"

namespace Settings {

std::shared_ptr<QSettings> loadSettings(const QString &filename,
                                        ConfigurationParameters &params) {
  auto settings = std::make_shared<QSettings>(filename, QSettings::IniFormat);
  if (settings->status() != QSettings::NoError) {
    throw std::invalid_argument{"error while opening configuration file"};
  }
  //  _settings->beginGroup("General");
  auto db_name{settings->value("DBName")};
  if (!db_name.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database name, check configuration file"};
  }
  params.db_name = db_name.value<QString>();
  auto db_host{settings->value("DBHost")};
  if (!db_host.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database server hostname, check configuration file"};
  }
  params.db_host = db_host.value<QString>();
  auto db_user{settings->value("DBUser")};
  if (!db_user.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database user name, check configuration file"};
  }
  params.db_user = db_user.value<QString>();
  auto db_password{settings->value("DBPassword")};
  if (!db_password.isValid()) {
    throw std::invalid_argument{
        "cannot retrieve database user password, check configuration file"};
  }
  params.db_password = db_password.value<QString>();

  return settings;
}

} // namespace Settings
