#include "CommandLineInterface.h"
#include "Database.h"
#include "Parsing.h"
#include <iostream>
#include <string>

//'Ανθρωπος

// using namespace std;

void CommandLineInterface::parseCommand() {
  std::string s;
  if (_currentLoginID != -1) {
    std::cout << "[" << _username << "] ";
    getline(std::cin, s);
  } else
    getline(std::cin, s);
  if (startWith("!", s)) {
    std::string command = splitBy(" ", s);
    if (command == "!login") {
      parseLogin(s);
      if (!s.empty()) {
        std::cout << "illegal parameter(s): " << s << std::endl;
        return;
      }
      if (!callLogin())
        return;
      showCountPM();
      showChatMsgs();
    } else if (command == "!logout") {
      if (!s.empty()) {
        std::cout << "illegal parameter(s): " << s << std::endl;
        return;
      }
      if (_currentLoginID == -1) {
        std::cout << "already logged out" << std::endl;
        return;
      }
      callLogout();
    } else if (command == "!exit") {
      if (!s.empty()) {
        std::cout << "illegal parameter(s): " << s << std::endl;
        return;
      }
      callExit();
    } else if (command == "!register") {
      parseLogin(s);
      if (!s.empty()) {
        std::cout << "illegal parameter(s): " << s << std::endl;
        return;
      }
      if (!callRegister())
        return;
      showChatMsgs();
    } else if (command == "!pm") {
      if (_currentLoginID == -1) {
        std::cout << "Please log in to write to other users!" << std::endl;
        return;
      }
      if (!parsePM(s))
        return;
      callPM(s);
    } else if (command == "!getPM") {
      if (_currentLoginID == -1) {
        std::cout << "Please log in to receive messages!" << std::endl;
        return;
      }
      callGetPM();
    } else {
      std::cout << "unknown command: " << command << std::endl;
      return;
    }
  } else {
    if (_currentLoginID == -1) {
      std::cout << "Please log in to write to the chat!" << std::endl;
      return;
    }
    if (!s.empty()) {
      _db.addChatMessage(_username, s);
      std::cout << "<" << _username << "> : " << s << std::endl;
    }
  }
}

void CommandLineInterface::parseLogin(std::string &s) {
  if (s.empty()) {
    std::cout << "enter username" << std::endl;
    getline(std::cin, s);
    _username = splitBy(" ", s);
  } else {
    _username = splitBy(" ", s);
  }
  if (!correctName(_username)) {
    std::cout << "incorrect username: " << _username << std::endl;
    return;
  }
  if (s.empty()) {
    std::cout << "enter password" << std::endl;
    getline(std::cin, s);
    _password = splitBy(" ", s);
  } else {
    _password = splitBy(" ", s);
  }
}

bool CommandLineInterface::parsePM(std::string &s) {
  if (startWith("(", s)) {
    skipuntil("(", s);
    std::string target = splitBy(")", s);
    if (!correctName(target)) {
      std::cout << "incorrect user name " << target << std::endl;
      return false;
    }
    _PMDest = target;
  } else if (_PMDest == "") {
    std::string target;
    std::cout << "enter user name who received message" << std::endl;
    getline(std::cin, target);
    if (!correctName(target)) {
      std::cout << "incorrect user name " << target << std::endl;
      return false;
    }
    _PMDest = target;
  }
  return true;
}

bool CommandLineInterface::callRegister() {
  _PMDest = "";
  _currentLoginID = _db.addUser(_username, _password);
  _password = "";
  if (_currentLoginID < 0)
    return false;
  return true;
}

bool CommandLineInterface::callLogin() {
  _PMDest = "";
  _currentLoginID = _db.checkPassword(_username, _password);
  _password = "";
  if (_currentLoginID == -1) {
    std::cout << "Login/password incorrect" << std::endl;
    return false;
  }
  return true;
}

void CommandLineInterface::callLogout() {
  _currentLoginID = -1;
  _username = "";
  _PMDest = "";
}

void CommandLineInterface::callExit() { _exit = true; }

void CommandLineInterface::callPM(std::string message) {
  if (_username == _PMDest) {
    std::cout << "self-message" << std::endl;
  }
  if (!_db.addPrivateMessage(_username, _PMDest, message)) {
    std::cout << "user " << _PMDest << " not found" << std::endl;
    return;
  }
}

void CommandLineInterface::callGetPM() {
  std::vector<Message> msgs = _db.getPrivateMessage(_currentLoginID);
  std::cout << "private messages:" << std::endl;
  for (auto &m : msgs) {
    std::cout << "<" << m.getSender() << ">: " << m.getText() << std::endl;
  }
  std::cout << "---" << std::endl;
}

void CommandLineInterface::showChatMsgs() {
  std::vector<std::string> chatmsgs = _db.getChatMessages();
  std::cout << "chat messages:" << std::endl;
  for (auto &m : chatmsgs) {
    std::cout << m << std::endl;
  }
  std::cout << "---" << std::endl;
}

bool CommandLineInterface::shouldExit() { return _exit; }

void CommandLineInterface::showCountPM() {
  std::vector<Message> pm = _db.getPrivateMessage(_currentLoginID);
  int countpm = pm.size();
  if (countpm)
    std::cout << "You have " << pm.size() << " private messages." << std::endl;
}
