#include "Database.h"
#include "Parsing.h"
#include "qmessagebox.h"
#include "sha1.h"
#include <QSqlQuery>
#include <memory>

//'Ανθρωπος

int Database::searchUserByName(std::string username) {
  auto uit = _usersMapByName.find(username);
  if (uit != _usersMapByName.end())
    return uit->second;
  return -1;
}

std::vector<std::string> Database::getUserList() const {
  std::vector<std::string> userList;
  for (auto user : _usersMapByName) {
    userList.push_back(user.first);
  }
  return userList;
}

std::string Database::getUserName(int userId) const {
  for (auto it = _usersMapByName.begin(); it != _usersMapByName.end(); ++it) {
    if (it->second == userId)
      return it->first;
  }
  return "";
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
  QMessageBox::about(nullptr, "test", "open");
  _db->close();
}

int Database::addUser(std::string username, std::string password) {
  std::shared_lock sl{_mtx};
  if (!correctName(username))
    return -1;
  auto uit = _usersMapByName.find(username);
  if (uit != _usersMapByName.end())
    return -2;
  User newUser = User(username, sha1(password));
  _users.push_back(newUser);
  _usersMapByName.insert({username, newUser.getUserID()});
  return newUser.getUserID();
}

int Database::checkPassword(std::string username, std::string password) {
  int result = -1;
  Hash passHash = sha1(password);
  for (const auto &u : _users) {
    result = u.checklogin(username, passHash);
    if (result != -1)
      return result;
  }
  return result;
}

void Database::addChatMessage(std::string sender, std::string text) {
  _messages.push_back(Message(sender, text));
}

bool Database::addPrivateMessage(std::string sender, std::string target,
                                 std::string message) {
  int targetUser = searchUserByName(target);
  if (targetUser < 0) {
    return false;
  }
  _messages.push_back(Message(sender, targetUser, message));
  return true;
}

std::vector<std::string> Database::getChatMessages() {
  std::vector<std::string> strings;
  for (const auto &m : _messages) {
    if (m.getDest() == -1) {
      strings.push_back("<" + m.getSender() + ">: " + m.getText());
    }
  }
  return strings;
}

std::vector<Message> Database::getPrivateMessage(int userID) {
  std::vector<Message> strings;
  // int userID = searchUserByName(username);
  for (auto &m : _messages) {
    if (userID == -1 && m.getDest() != -1)
      strings.push_back(m);
    else if (userID != -1 && m.getDest() == userID)
      strings.push_back(m);
  }
  return strings;
}
