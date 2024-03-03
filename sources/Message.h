#pragma once
#include <string>

class Message {
  static int num; //уникальный номер, который получает id
  static int messageCounter;
  int _id;
  std::string _sender;
  int _destID;
  std::string _text;
  std::string _time;

public:
  Message();
  Message(const std::string &writer, const std::string &text,
          const std::string &time); //сообщение в чат
  Message(const std::string &writer, int target, const std::string &text,
          const std::string &time); //личное сообщение

  bool searchByTarget(int) const;
  std::string getSender() const;
  int getDest() const;
  int getID() const;
  std::string getText() const;
  const std::string &getTime() const;
};
