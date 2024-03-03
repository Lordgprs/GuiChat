#include "Message.h"
int Message::num = 0;
int Message::messageCounter = 0;
//сообщение в чат
const std::string &Message::getTime() const
{
  return _time;
}

Message::Message() : _id(num), _sender(""), _destID(0), _text("") {}
Message::Message(const std::string &writer, const std::string &text,
                 const std::string &time)
    : _id(++num), _sender(writer), _destID(-1), _text(text), _time{time} {
  messageCounter++;
}
//личное сообщение
Message::Message(const std::string &writer, int target, const std::string &text,
                 const std::string &time)
    : _id(++num), _sender(writer), _destID(target), _text(text), _time{time} {
  messageCounter++;
}

bool Message::searchByTarget(int n) const { return (_destID == n); }

std::string Message::getSender() const { return _sender; }

int Message::getDest() const { return _destID; }

std::string Message::getText() const { return _text; }

int Message::getID() const { return _id; }
