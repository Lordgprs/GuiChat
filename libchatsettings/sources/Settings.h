#pragma once

#include <QSettings>
#include <QString>
#include <memory>

namespace Settings {

struct ConfigurationParameters {
  QString db_name;
  QString db_host;
  QString db_user;
  QString db_password;
};

std::shared_ptr<QSettings>
loadSettings(const QString &filename, Settings::ConfigurationParameters &params);

} // namespace Settings
