#include "User.h"
int User::userCounter = 1;

User::User() : _id(0), _login(""), _username(""), _passwordHash()
{
}

User::User(std::string username, Hash passHash) :_id(++userCounter), _login(username), _username(username), _passwordHash(passHash)
{
}

int User::checklogin(const std::string& login, const Hash& passHash) const
{
	if ((_login == login) && (_passwordHash == passHash)) return _id;
	return -1;
}

std::string& User::getuserName()
{
	return _username;
}

int User::getUserID() const
{
	return _id;
}
