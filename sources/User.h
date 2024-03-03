#pragma once
#include "sha1.h"
#include <string>
// using namespace std;

class User {
  static int userCounter;
  int _id;
  std::string _login;
  std::string _username;
  Hash _passwordHash;

public:
  User();
  User(std::string username, Hash passHash);
  int checklogin(const std::string &login, const Hash &passHash) const;
  std::string &getuserName();
  int getUserID() const;
};
