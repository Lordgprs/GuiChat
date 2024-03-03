#include "DbException.h"

DbException::DbException() {}

DbException::DbException(const std::string &message) : _message{message} {}

const char *DbException::what() const noexcept { return _message.c_str(); }
