#include "User.h"

User::User(int id, const std::string &name, bool active)
    : _active{active}, _id{id}, _name{name} {}

bool User::operator==(const User &other) const {
  bool result{true};
  result = result && other._active == _active;
  result = result && other._id == _id;
  result = result && other._name == _name;

  return result;
}

bool User::isActive() const { return _active; }

void User::setActive(bool newActive) { _active = newActive; }

int User::getId() const { return _id; }

void User::setId(int newId) { _id = newId; }

const std::string &User::getName() const { return _name; }

void User::setName(const std::string &newName) { _name = newName; }
