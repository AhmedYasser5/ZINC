#include "lexer.hpp"
#include <iostream>
#include <string>

bool Token::operator==(TOKEN_TYPE type) const { return this->type == type; }
bool Token::operator!=(TOKEN_TYPE type) const { return !(*this == type); }

Token::Token(std::string Text, TOKEN_TYPE T) {
  text = Text;
  type = T;
}
Token::Token() {
  text = " ";
  type = NONE;
  line = 0;
}
Lexer::Lexer(std::string s) {
  source = s;
  curChar = ' ';
  curPos = -1;
  curLine = 1;
  nextChar();
}
void Token::stringToType(std::string s) {

  if (s == "LABEL") {
    text = s;
    type = LABEL;
  } else if (s == "GOTO") {
    text = s;
    type = GOTO;
  } else if (s == "INPUT") {
    text = s;
    type = INPUT;
  } else if (s == "LET") {
    text = s;
    type = LET;
  } else if (s == "IF") {
    text = s;
    type = IF;
  } else if (s == "THEN") {
    text = s;
    type = THEN;
  } else if (s == "ENDIF") {
    text = s;
    type = ENDIF;
  } else if (s == "WHILE") {
    text = s;
    type = WHILE;
  } else if (s == "REPEAT") {
    text = s;
    type = REPEAT;
  } else if (s == "ENDWHILE") {
    text = s;
    type = ENDWHILE;
  } else if (s == "PRINT") {
    text = s;
    type = PRINT;
  } else if (s == "ELSEIF") {
    text = s;
    type = ELSEIF;
  } else if (s == "ELSE") {
    text = s;
    type = ELSE;
  } else {
    text = s;
    type = IDENT;
  }
}
void Lexer::nextChar() {
  curPos += 1;
  if (curPos >= int(source.size()))
    curChar = '\0'; // EOF - End of file
  else
    curChar = source[curPos];
}
char Lexer::Peek() {
  if (curPos + 1 >= int(source.size()))
    return '\0';
  else
    return source[curPos + 1];
}
Token Lexer::getToken() {

  Token token;
  token.line = curLine;
  int startPos;
  skipWhitespace();
  skipComment();
  if (curChar == '+') {
    token.type = PLUS;
    token.text = curChar;
  } else if (curChar == '-') {
    token.type = MINUS;
    token.text = curChar;
  } else if (curChar == '*') {
    token.type = ASTERISK;
    token.text = curChar;
  } else if (curChar == '/') {
    token.type = SLASH;
    token.text = curChar;
  } else if (curChar == '=') {
    if (Peek() == '=') {
      token.text = token.text + curChar + '=';
      token.type = EQEQ;
      nextChar();
    } else {
      token.text = curChar;
      token.type = EQ;
    }
  } else if (curChar == '>') {
    if (Peek() == '=') {
      token.text = token.text + curChar + '=';
      token.type = GTEQ;
      nextChar();
    } else {
      token.text = curChar;
      token.type = GT;
    }
  } else if (curChar == '<') {
    if (Peek() == '=') {
      token.text = token.text + curChar + '=';
      token.type = LTEQ;
      nextChar();
    } else {
      token.text = curChar;
      token.type = LT;
    }
  } else if (curChar == '!') {
    if (Peek() == '=') {
      token.text = token.text + curChar + '=';
      token.type = NOTEQ;
      nextChar();
    } else {
      std::cout << "Error at line " << curLine << std::endl;
      std::cout << "Illegal Token" << std::endl;
      throw 3;
    }
  }

  else if (isdigit(curChar)) {
    startPos = curPos;
    while (isdigit(Peek())) {
      nextChar();
    }
    if (Peek() == '.') {
      nextChar();
      if (!isdigit(Peek())) {
        std::cout << "Error at line " << curLine << std::endl;
        std::cout << "Must at least have one number after decimal point"
                  << std::endl;
        throw 3;
      }
      while (isdigit(Peek())) {
        nextChar();
      }
    }
    token.type = NUMBER;
    token.text = source.substr(startPos, curPos - startPos + 1);
  } else if (curChar == '\n') {
    token.type = NEWLINE;
    token.text = curChar;
    curLine++;
  } else if (curChar == '\0') {
    token.type = _EOF;
    token.text = curChar;
  }
  // the next one is for capturing strings
  else if (curChar == '\"') {
    // get the characters between quotations
    nextChar();
    startPos = curPos;
    while (curChar != '\"') {
      if (curChar == '\r' || curChar == '\n' || curChar == '\t' ||
          curChar == '\\' || curChar == '%') {
        std::cout << "Error at line " << curLine << std::endl;
        std::cout << "Illegal character in string" << std::endl;
        throw 3;
      }
      nextChar();
    }
    token.type = STRING;
    token.text = source.substr(startPos, curPos - startPos);
  } else if (isalpha(curChar)) {
    startPos = curPos;
    while (isalnum(Peek())) {
      nextChar();
    }
    token.text = source.substr(startPos, curPos - startPos + 1);
    token.stringToType(token.text);
  } else {
    std::cout << "Error at line " << curLine << std::endl;
    std::cout << "unidentified Token " << curChar << std::endl;
    throw 3;
  }
  nextChar();
  return token;
}
void Lexer::skipWhitespace() {
  while (curChar == ' ' || curChar == '\r' || curChar == '\t')
    nextChar();
}
void Lexer::skipComment() {
  if (curChar == '#') {
    while (curChar != '\n')
      nextChar();
  }
}
