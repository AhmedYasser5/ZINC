#pragma once
#include <string>
// EOF already exists and it's equal to -1
enum TOKEN_TYPE {
  NONE = -2,
  _EOF = -1,
  NEWLINE = 0,
  NUMBER = 1,
  IDENT = 2,
  STRING = 3,
  // Keywords
  LABEL = 101,
  GOTO = 102,
  PRINT = 103,
  INPUT = 104,
  LET = 105,
  IF = 106,
  THEN = 107,
  ENDIF = 108,
  WHILE = 109,
  REPEAT = 110,
  ENDWHILE = 111,
  ELSEIF = 112,
  ELSE = 113,
  // Operators
  EQ = 201,
  PLUS = 202,
  MINUS = 203,
  ASTERISK = 204,
  SLASH = 205,
  EQEQ = 206,
  NOTEQ = 207,
  LT = 208,
  LTEQ = 209,
  GT = 210,
  GTEQ = 211
};

class Token {
public:
  TOKEN_TYPE type;
  std::string text;
  int line;
  Token(std::string Text, TOKEN_TYPE T);
  Token();
  bool operator==(TOKEN_TYPE type) const;
  bool operator!=(TOKEN_TYPE type) const;
  void stringToType(std::string s);
};
class Lexer {
public:
  std::string source;
  char curChar;
  int curPos;
  int curLine;
  Lexer(std::string s);
  void nextChar();
  char Peek();
  Token getToken();
  void skipWhitespace();
  void skipComment();
};
