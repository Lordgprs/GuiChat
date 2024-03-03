#include "Parsing.h"
#include <string>

// using namespace std;

bool startWith(std::string tmpl, std::string &s) {
  if (tmpl.length() > s.length())
    return false;
  return !s.compare(0, tmpl.length(), tmpl);
}

bool startWith(std::string tmpl, const char *s) {
  if (tmpl.empty())
    return false;
  //проверяем равенство каждого символа шаблона с символами строки
  for (size_t iptr = 0; iptr < tmpl.length(); ++iptr) {
    if (s[iptr] == '\0')
      return false; //достигнут конец строки
    if (tmpl[iptr] != s[iptr])
      return false; //есть различия
  }
  return true; //все символы прошли тест на равенство - значит различий нет
}

int findFirst(std::string tmpl, std::string &s) {
  const char *ss = s.c_str();
  int index = -1;
  //ищем позицию начиная с которой встречается шаблон
  for (size_t shift = 0; shift < (s.length() - tmpl.length() + 1); ++shift) {
    //чтобы не создавать копии подстрок после чего проводить сравнения - просто
    //перемещаем указатель
    if (startWith(tmpl, ss + shift)) {
      index = shift;
      break;
    }
  }
  return index;
}

std::string splitBy(std::string tmpl, std::string &s) {
  int index = findFirst(tmpl, s);
  if (index ==
      -1) //шаблона нет - перемещаем всё в вывод, очищаем исходную строку.
  {
    std::string temp = s;
    s.clear();
    return temp;
  }
  std::string temp = s.substr(0, index);
  s.erase(0, index + tmpl.length());
  return temp;
}

void skipuntil(std::string tmpl, std::string &s) {
  int index = findFirst(tmpl, s);
  if (index == -1)
    s.clear();
  s.erase(0, index + tmpl.length());
}

std::string segment(std::string from, std::string to, std::string s) {
  skipuntil(from, s);
  return splitBy(to, s);
}

bool isDigit(char c) { return (c >= '0') && (c <= '9'); }

bool isLetter(char c) {
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

bool correctName(std::string s) {
  if (s.empty())
    return false;
  if (!isLetter(s[0]))
    return false;
  for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
    char c = (*it);
    if ((!isDigit(c)) && (!isLetter(c)) && (c != '_'))
      return false;
  }
  return true;
}
