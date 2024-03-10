#pragma once
#include <string>

class Message {
  int _id{-1};
  std::string _sender;
  int _destID;
  std::string _text;
  std::string _time;

public:
  Message();
  Message(int id, const std::string &writer, const std::string &text,
          const std::string &time); //сообщение в чат
  Message(int id, const std::string &writer, int target,
          const std::string &text,
          const std::string &time); //личное сообщение
  bool operator==(const Message &) const;

  bool searchByTarget(int) const;
  std::string getSender() const;
  int getDest() const;
  int getID() const;
  std::string getText() const;
  const std::string &getTime() const;
};
