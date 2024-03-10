#include "Message.h"

//сообщение в чат
const std::string &Message::getTime() const { return _time; }

Message::Message() : _sender(""), _destID(0), _text("") {}
Message::Message(int id, const std::string &writer, const std::string &text,
                 const std::string &time)
    : _id(id), _sender(writer), _destID(-1), _text(text), _time{time} {}
//личное сообщение
Message::Message(int id, const std::string &writer, int target,
                 const std::string &text, const std::string &time)
    : _id(id), _sender(writer), _destID(target), _text(text), _time{time} {}

bool Message::operator==(const Message &other) const {
  bool result{true};
  //  result = result && other._id == _id;
  result = result && other._destID == _destID;
  result = result && other._sender == _sender;
  result = result && other._text == other._text;
  result = result && other._time == other._time;
  return result;
}

bool Message::searchByTarget(int n) const { return (_destID == n); }

std::string Message::getSender() const { return _sender; }

int Message::getDest() const { return _destID; }

std::string Message::getText() const { return _text; }

int Message::getID() const { return _id; }
