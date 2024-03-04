#pragma once
#include <string>

/*
  проверяет, что строка начинается с шаблона
*/
bool startWith(std::string tmpl, std::string &s);

/*
  проверяет, что строка начинается с шаблона
  версия для c-style строки (нужно для оптимизации других функций)
*/
bool startWith(std::string tmpl, const char *s);

/*
  находит позицию начиная с которой встречается шаблон. Возвращает -1 если
  шаблона нет.
*/
int findFirst(std::string tmpl, std::string &s);

/*
  делит строку на подстроку до шаблона (которую возвращает) и после (до которой
  обрезает переданную по ссылке)
*/
std::string splitBy(std::string tmpl, std::string &s);

/*
  удаляет строку до шаблона. При несовпадении очищает строку полностью.
*/
void skipuntil(std::string tmpl, std::string &s);

/*
  возвращает участок строки от шаблона from до шаблона to
*/
std::string segment(std::string from, std::string to, std::string s);

/*
  определяет, является ли символ буквой
*/
bool isLetter(char c);

/*
  определяет, является ли символ цифрой
*/
bool isDigit(char c);

/*
  проверяет корректность строки в качестве имени пользователя
*/
bool correctName(std::string s);
