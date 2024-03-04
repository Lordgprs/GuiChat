#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include <QString>
#include <stdexcept>
#include <string>

class DbException : public std::exception {
public:
  DbException();
  DbException(const std::string &message);
  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

private:
  std::string _message;
};

#endif // DBEXCEPTION_H
