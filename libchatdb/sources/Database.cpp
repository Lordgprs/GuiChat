#include "Database.h"
#include "DbException.h"
#include "Parsing.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <memory>

int Database::searchUserByName(std::string username) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  int uid{-1};
  QSqlQuery query{*_db};

  query.prepare("SELECT id FROM users WHERE name = ?");
  query.addBindValue(QString::fromStdString(username));
  query.setForwardOnly(true);
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"can not get user id by name"};
  }
  if (query.next()) {
    uid = query.value(0).value<int>();
  }

  return uid;
}

std::vector<User> Database::getUserList(bool including_inactive) const {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  std::vector<User> userList;
  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  bool sql_result{false};
  if (including_inactive) {
    sql_result = query.exec("SELECT id, name, active FROM users ORDER BY id");
  } else {
    sql_result = query.exec(
        "SELECT id, name, active FROM users WHERE active ORDER BY id");
  }
  if (!sql_result) {
    throw DbException{"can not get user list"};
  }
  while (query.next()) {
    userList.emplace_back(query.value(0).value<int>(),
                          query.value(1).value<QString>().toStdString(),
                          query.value(2).value<bool>());
  }

  return userList;
}

std::string Database::getUserName(int userId) const {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  QString result;
  QSqlQuery query(*_db);
  query.prepare("SELECT name FROM users WHERE id = ?");
  query.addBindValue(userId);
  query.setForwardOnly(true);
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"cannot get username by id"};
  }
  if (query.next()) {
    result = query.value(0).value<QString>();
  }

  return result.toStdString();
}

Database::Database(const QString &db_name, const QString &db_host,
                   const QString &db_user, const QString &db_password)
    : _messages{}, _db_name{db_name}, _db_host{db_host}, _db_user{db_user},
      _db_password{db_password} {
  static auto db{QSqlDatabase::addDatabase("QPSQL")};

  db.setDatabaseName(_db_name);
  db.setHostName(_db_host);
  db.setUserName(_db_user);
  db.setPassword(_db_password);
  db.setConnectOptions("connect_timeout=3");
  auto isOpen{db.open()};
  if (!isOpen) {
    throw std::invalid_argument{"cannot connect to database"};
  }
  _db = &db;
}

Database::~Database() {
  if (_db) {
    _db->close();
  }
}

int Database::addUser(std::string username, std::string password) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  bool sql_result;
  int userId{-2};
  if (!correctName(username)) {
    return -1;
  }
  QSqlQuery query{*_db};
  QString sql{"SELECT id, name, password_hash FROM users WHERE active AND "
              "name = ? "};
  query.prepare(sql);
  query.addBindValue(QString::fromStdString(username));
  sql_result = query.exec();
  if (!sql_result) {
    throw DbException("can not get user list: " + sql.toStdString());
  }
  if (query.size() > 0) {
    return userId;
  }
  query.clear();
  query.prepare("INSERT INTO users(name, password_hash) VALUES (?, "
                "encode(sha256(?), 'hex'))");
  query.addBindValue(QString::fromStdString(username));
  query.addBindValue(QString::fromStdString(password));
  sql_result = query.exec();
  if (!sql_result || query.numRowsAffected() != 1) {
    throw DbException{"can not add user to database"};
  }
  query.clear();
  query.prepare("SELECT id FROM users WHERE name = ?");
  query.addBindValue(QString::fromStdString(username));
  query.setForwardOnly(true);
  sql_result = query.exec();
  if (!sql_result || query.size() == 0) {
    throw DbException{"can not get ID of new user from database"};
  }
  while (query.next()) {
    userId = query.value(0).value<int>();
  }

  return userId;
}

int Database::checkPassword(std::string username, std::string password) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  int result = -1;
  QSqlQuery query(*_db);
  query.prepare("SELECT id FROM users WHERE name = ? AND password_hash = "
                "encode(sha256(?), 'hex')");
  query.addBindValue(QString::fromStdString(username));
  query.addBindValue(QString::fromStdString(password));
  query.setForwardOnly(true);
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"can not check user password"};
  }
  while (query.next()) {
    result = query.value(0).value<int>();
  }
  return result;
}

bool Database::addChatMessage(std::string sender, std::string message) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  int senderId{searchUserByName(sender)};
  if (senderId < 0) {
    return false;
  }
  std::scoped_lock sl{_mtx};
  QSqlQuery query(*_db);
  query.prepare("INSERT INTO messages (sender_id, message_text) "
                "VALUES(?, ?)");
  query.addBindValue(senderId);
  query.addBindValue(QString::fromStdString(message));
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    throw DbException{"can not add broadcast message to database"};
  }
  return true;
}

bool Database::addPrivateMessage(std::string sender, std::string target,
                                 std::string message) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  int senderId{searchUserByName(sender)};
  int targetId{searchUserByName(target)};
  if (senderId < 0 || targetId < 0) {
    return false;
  }
  std::scoped_lock sl{_mtx};
  QSqlQuery query(*_db);
  query.prepare("INSERT INTO messages (sender_id, receiver_id, message_text) "
                "VALUES(?, ?, ?)");
  query.addBindValue(senderId);
  query.addBindValue(targetId);
  query.addBindValue(QString::fromStdString(message));
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    throw DbException{"can not add private message to database"};
  }

  return true;
}

bool Database::removeMessage(int id) const {
  if (id < 0) {
    return false;
  }

  qWarning() << "Removing message #" << id;
  QSqlQuery query(*_db);
  query.prepare("DELETE FROM messages WHERE id = ?");
  query.addBindValue(id);
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    return false;
  }

  return true;
}

bool Database::removeUser(int id) const {
  if (id < 0) {
    return false;
  }

  qWarning() << "Removing user #" << id;
  QSqlQuery query(*_db);
  query.prepare("DELETE FROM users WHERE id = ?");
  query.addBindValue(id);
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    return false;
  }

  return true;
}

bool Database::disableUser(int id) const {
  if (id < 0) {
    return false;
  }

  qWarning() << "Disabling user #" << id;
  QSqlQuery query(*_db);
  query.prepare("UPDATE users SET active = false WHERE id = ?");
  query.addBindValue(id);
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    return false;
  }

  return true;
}

bool Database::enableUser(int id) const {
  if (id < 0) {
    return false;
  }

  qWarning() << "Enabling user #" << id;
  QSqlQuery query(*_db);
  query.prepare("UPDATE users SET active = true WHERE id = ?");
  query.addBindValue(id);
  bool sql_result{query.exec()};
  if (!sql_result || query.numRowsAffected() != 1) {
    return false;
  }

  return true;
}

std::vector<std::string> Database::getChatMessages() {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  std::vector<std::string> strings;
  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  query.prepare("SELECT "
                "users.name, "
                "messages.message_text, "
                "to_char(messages.sent, 'HH24:MI') "
                "FROM messages "
                "JOIN users ON users.id = messages.sender_id "
                "WHERE messages.receiver_id IS NULL");
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"can not get broadcast messages"};
  }
  while (query.next()) {
    QString message = query.value(2).value<QString>() + ": <" +
                      query.value(0).value<QString>() + "> " +
                      query.value(1).value<QString>();
    strings.push_back(message.toStdString());
  }
  return strings;
}

std::vector<Message> Database::getAllMessages() {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }

  std::scoped_lock sl{_mtx};
  std::vector<Message> messages;

  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  query.prepare("SELECT "
                "users.name, "
                "COALESCE(messages.receiver_id, -1), "
                "messages.message_text, "
                "to_char(messages.sent, 'DD.MM HH24:MI'), "
                "messages.id "
                "FROM messages "
                "JOIN users ON users.id = messages.sender_id "
                "ORDER BY messages.sent DESC");
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"can not get private messages"};
  }
  while (query.next()) {
    QString message = query.value(2).value<QString>() + ": <" +
                      query.value(0).value<QString>() + "> " +
                      query.value(1).value<QString>();
    messages.emplace_back(query.value(4).value<int>(),
                          query.value(0).value<QString>().toStdString(),
                          query.value(1).value<int>(),
                          query.value(2).value<QString>().toStdString(),
                          query.value(3).value<QString>().toStdString());
  }
  return messages;
}

std::vector<Message> Database::getPrivateMessages(int userID) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::scoped_lock sl{_mtx};
  std::vector<Message> strings;

  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  query.prepare("SELECT "
                "users.name, "
                "COALESCE(messages.receiver_id, -1), "
                "messages.message_text, "
                "to_char(messages.sent, 'DD.MM HH24:MI'), "
                "messages.id "
                "FROM messages "
                "JOIN users ON users.id = messages.sender_id "
                "WHERE messages.receiver_id = ? "
                "OR "
                "(messages.sender_id = ? AND "
                "messages.receiver_id IS NOT NULL)"
                "ORDER BY messages.sent DESC");
  query.addBindValue(userID);
  query.addBindValue(userID);
  bool sql_result{query.exec()};
  if (!sql_result) {
    throw DbException{"can not get private messages"};
  }
  while (query.next()) {
    QString message = query.value(2).value<QString>() + ": <" +
                      query.value(0).value<QString>() + "> " +
                      query.value(1).value<QString>();
    strings.emplace_back(query.value(4).value<int>(),
                         query.value(0).value<QString>().toStdString(),
                         query.value(1).value<int>(),
                         query.value(2).value<QString>().toStdString(),
                         query.value(3).value<QString>().toStdString());
  }
  return strings;
}
