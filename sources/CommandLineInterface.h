#pragma once
#include "Database.h"
#include <string>
// using namespace std;

class CommandLineInterface {
  int _currentLoginID = -1;
  std::string _username = "";
  std::string _password;
  std::string _PMDest = "";
  Database _db;
  bool _exit = false;
  void parseLogin(std::string &);
  bool parsePM(std::string &);
  bool callRegister();
  bool callLogin();
  void callLogout();
  void callExit();
  void callPM(std::string);
  void callGetPM();
  void showChatMsgs();
  void showCountPM();

public:
  void parseCommand();
  bool shouldExit();
};
