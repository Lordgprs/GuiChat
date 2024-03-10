#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
  User() = delete;
  User(int id, const std::string &name, bool active);

  bool operator==(const User &other) const;

  bool isActive() const;
  void setActive(bool newActive);

  int getId() const;
  void setId(int newId);

  const std::string &getName() const;
  void setName(const std::string &newName);

private:
  bool _active{true};
  int _id{0};
  std::string _name;
};

#endif // USER_H
