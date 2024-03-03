#include "Database.h"
#include "DbException.h"
#include "Parsing.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <memory>

int Database::searchUserByName(std::string username) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::shared_lock sl{_mtx};
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

std::vector<std::string> Database::getUserList() const {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::shared_lock sl{_mtx};
  std::vector<std::string> userList;
  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  bool sql_result{query.exec("SELECT name FROM users ORDER BY id")};
  if (!sql_result) {
    throw DbException{"can not get user list"};
  }
  while (query.next()) {
    userList.push_back(query.value(0).value<QString>().toStdString());
  }

  return userList;
}

std::string Database::getUserName(int userId) const {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::shared_lock sl{_mtx};
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
    : _users{}, _messages{}, _db_name{db_name}, _db_host{db_host},
      _db_user{db_user}, _db_password{db_password} {
  static auto db{QSqlDatabase::addDatabase("QPSQL")};
  _db = &db;
  _db->setDatabaseName(_db_name);
  _db->setHostName(_db_host);
  _db->setUserName(_db_user);
  _db->setPassword(_db_password);
  auto isOpen{_db->open()};
  if (!isOpen) {
    throw std::invalid_argument{"cannot connect to database"};
  }
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
  std::unique_lock ul{_mtx};
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
  std::shared_lock sl{_mtx};
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
  std::unique_lock ul{_mtx};
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
  std::unique_lock ul{_mtx};
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

std::vector<std::string> Database::getChatMessages() {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::shared_lock sl{_mtx};
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

std::vector<Message> Database::getPrivateMessages(int userID) {
  if (!_db) {
    throw DbException{"database connection is not established"};
  }
  std::shared_lock sl{_mtx};
  std::vector<Message> strings;
  // int userID = searchUserByName(username);
  QSqlQuery query(*_db);
  query.setForwardOnly(true);
  query.prepare("SELECT "
                "users.name, "
                "COALESCE(messages.receiver_id, -1), "
                "messages.message_text, "
                "to_char(messages.sent, 'HH24:MI') "
                "FROM messages "
                "JOIN users ON users.id = messages.sender_id "
                "WHERE messages.receiver_id = ? "
                "OR "
                "(messages.sender_id = ? AND "
                "messages.receiver_id IS NOT NULL)");
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
    strings.emplace_back(query.value(0).value<QString>().toStdString(),
                         query.value(1).value<int>(),
                         query.value(2).value<QString>().toStdString(),
                         query.value(3).value<QString>().toStdString());
  }
  return strings;
}
