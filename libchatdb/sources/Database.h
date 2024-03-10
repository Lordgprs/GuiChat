#pragma once

#include "Message.h"
#include "User.h"
#include <QString>
#include <QtSql/QSqlDatabase>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Database final {
  std::vector<Message> _messages;
  std::unordered_map<std::string, int> _usersMapByName;
  QSqlDatabase *_db{nullptr};
  QString _db_name{"chat"};
  QString _db_host{"localhost"};
  QString _db_user{"chat"};
  QString _db_password{"12345"};
  mutable std::mutex _mtx;

public:
  int searchUserByName(std::string);
  std::vector<User> getUserList(bool including_inactive = false) const;
  std::string getUserName(int userId) const;
  Database(const QString &db_name, const QString &db_host,
           const QString &db_user, const QString &db_password);
  ~Database();
  int addUser(std::string username, std::string password);
  int checkPassword(std::string username, std::string password);
  bool addChatMessage(std::string sender, std::string);
  bool addPrivateMessage(std::string sender, std::string target,
                         std::string message);
  bool removeMessage(int id) const;
  bool removeUser(int id) const;
  bool disableUser(int id) const;
  bool enableUser(int id) const;
  std::vector<std::string> getChatMessages(); //показать общие сообщения
  std::vector<Message> getAllMessages();
  std::vector<Message> getPrivateMessages(
      int userID = -1); //показать личные сообщения пользователю username
};
