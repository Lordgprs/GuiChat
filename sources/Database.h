#pragma once
#include "Message.h"
#include "User.h"
#include <QtSql/QSqlDatabase>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Database {
  std::vector<User> _users;
  std::vector<Message> _messages;
  std::unordered_map<std::string, int> _usersMapByName;
  int searchUserByName(std::string);
  QSqlDatabase *_db{nullptr};
  QString _db_name{"chat"};
  QString _db_host{"localhost"};
  QString _db_user{"chat"};
  QString _db_password{"12345"};
  std::shared_mutex _mtx;

public:
  std::vector<std::string> getUserList() const;
  std::string getUserName(int userId) const;
  Database(const QString &db_name, const QString &db_host,
           const QString &db_user, const QString &db_password);
  int addUser(std::string username, std::string password);
  int checkPassword(std::string username, std::string password);
  void addChatMessage(std::string sender, std::string);
  bool addPrivateMessage(std::string sender, std::string target,
                         std::string message);
  std::vector<std::string> getChatMessages(); //показать все сообщения
  std::vector<Message> getPrivateMessage(
      int userID = -1); //показать личные сообщения пользователю username
};
