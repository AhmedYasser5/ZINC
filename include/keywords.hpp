#pragma once
#include "base.hpp"
#include <string>
#include <vector>

#define Token int

/* 
 * Program
 * Block
 * Comparison
 * Expression
 * Term
 * Unary
 * IntLiteral
 * StringLiteral
 * DoubleLiteral
 * Identifer
 * Print
 * Assign
 * Declare
 */

class Program : public ASTNode {
private:
  std::vector<ASTNode *> _statements;

public:
  Program(std::vector<ASTNode *> vec) { _statements = vec; }

  ~Program() {
    for (auto it : _statements)
      delete it;
    _statements.clear();
  }

  void accept(Visitor *visitor) override;

  std::vector<ASTNode *> statements() const { return _statements; }
};

class Block : public ASTNode {
private:
  std::vector<ASTNode *> _statements;

public:
  Block(std::vector<ASTNode *> vec) { _statements = vec; }

  ~Block() {
    for (auto it : _statements)
      delete it;
    _statements.clear();
  }

  void accept(Visitor *visitor) override;

  std::vector<ASTNode *> statements() const { return _statements; }
};

class Comparison : public ASTNode {
private:
  Token _op;
  ASTNode *_left;
  ASTNode *_right;

public:
  Comparison(ASTNode *left, Token op, ASTNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Comparison() {
    delete _left;
    delete _right;
  }

  ASTNode *left() const { return _left; }

  ASTNode *right() const { return _right; }

  Token op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Expression : public ASTNode {
private:
  char _op;
  ASTNode *_left;
  ASTNode *_right;

public:
  Expression(ASTNode *left, char op, ASTNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Expression() {
    delete _left;
    delete _right;
  }

  ASTNode *left() const { return _left; }

  ASTNode *right() const { return _right; }

  char op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Term : public ASTNode {
private:
  char _op;
  ASTNode *_left;
  ASTNode *_right;

public:
  Term(ASTNode *left, char op, ASTNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Term() {
    delete _left;
    delete _right;
  }

  ASTNode *left() const { return _left; }

  ASTNode *right() const { return _right; }

  char op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Unary : public ASTNode {
private:
  char _op;
  ASTNode *_primary;

public:
  Unary(char op, ASTNode *primary) {
    this->_op = op;
    this->_primary = primary;
  }

  ~Unary() { delete _primary; }

  ASTNode *primary() const { return _primary; }

  char op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class DoubleLiteral : public ASTNode {
private:
  // The value
  double _value;

public:
  // A constructor that takes the value as an argument
  DoubleLiteral(double value) { _value = value; }

  // A function to accept a visitor
  void accept(Visitor *visitor) override;

  // A function to get the value
  double value() const { return _value; }
};

class StringLiteral : public ASTNode {
private:
  std::string _str;

public:
  StringLiteral(std::string str) { this->_str = str; }

  void accept(Visitor *visitor) override;

  std::string str() const { return _str; }
};

class Identifer : public ASTNode {
private:
  std::string _ident;

public:
  Identifer(std::string ident) { this->_ident = ident; }

  void accept(Visitor *visitor) override;

  std::string getIdent() const { return _ident; }
};

///////////////////////////////////////////////////////////////////////////////
class Print : public ASTNode {
private:
  ASTNode *_argument;

public:
  Print(ASTNode *argument) { this->_argument = argument; }

  ~Print() { delete _argument; }

  void accept(Visitor *visitor) override;

  ASTNode *argument() const { return _argument; }
};

class Declare : public ASTNode {
private:
  ASTNode *_ident;
  ASTNode *_expr;
  Token _data_type;

public:
  Declare(ASTNode *identifer, ASTNode *expr, Token data_type) {
    _ident = identifer;
    _expr = expr;
    _data_type = data_type;
  }

  ~Declare() {
    delete _expr;
    delete _ident;
  }

  void accept(Visitor *visitor) override;

  ASTNode *ident() const { return _ident; }

  ASTNode *expr() const { return _expr; }

  Token data_type() const { return _data_type; }
};
