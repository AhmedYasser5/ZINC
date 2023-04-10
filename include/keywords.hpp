#pragma once
#include "base.hpp"
#include "lexer.hpp"
#include <string>
#include <vector>

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
 * Identifier
 * Print
 * Assign
 * Declare
 */

class Block : public ASTNode {
private:
  std::vector<ASTNode *> _statements;

public:
  Block(std::vector<ASTNode *> vec) { _statements = vec; }

  ~Block() {
    for (auto it : _statements)
      delete it;
  }

  void accept(Visitor *visitor) override;

  const std::vector<ASTNode *> &statements() const { return _statements; }
};

class Unary : public MathNode {
private:
  Token _op;
  Primary *_primary;

public:
  Unary(Token op, Primary *primary) {
    this->_op = op;
    this->_primary = primary;
  }

  ~Unary() { delete _primary; }

  Primary *primary() const { return _primary; }

  Token op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Term : public MathNode {
private:
  Token _op;
  MathNode *_left;
  MathNode *_right;

public:
  Term(MathNode *left, Token op, MathNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Term() {
    delete _left;
    delete _right;
  }

  MathNode *left() const { return _left; }

  MathNode *right() const { return _right; }

  Token op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Expression : public MathNode {
private:
  Token _op;
  MathNode *_left;
  MathNode *_right;

public:
  Expression(MathNode *left, Token op, MathNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Expression() {
    delete _left;
    delete _right;
  }

  MathNode *left() const { return _left; }

  MathNode *right() const { return _right; }

  Token op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Comparison : public MathNode {
private:
  Token _op;
  MathNode *_left;
  MathNode *_right;

public:
  Comparison(MathNode *left, Token op, MathNode *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Comparison() {
    delete _left;
    delete _right;
  }

  MathNode *left() const { return _left; }

  MathNode *right() const { return _right; }

  Token op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class DoubleLiteral : public Primary {
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

  const std::string &str() const { return _str; }
};

class Identifier : public Primary {
private:
  std::string _ident;

public:
  Identifier(std::string ident) { this->_ident = ident; }

  void accept(Visitor *visitor) override;

  const std::string &ident() const { return _ident; }
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

class Let : public ASTNode {
private:
  Identifier *_ident;
  MathNode *_expr;

public:
  Let(Identifier *identifer, MathNode *expr) {
    _ident = identifer;
    _expr = expr;
  }

  ~Let() {
    delete _expr;
    delete _ident;
  }

  void accept(Visitor *visitor) override;

  Identifier *ident() const { return _ident; }

  MathNode *expr() const { return _expr; }
};

class If : public ASTNode {
private:
  Comparison *_comp;
  Block *_block;

public:
  If(Comparison *comp, Block *block) {
    _comp = comp;
    _block = block;
  }

  ~If() {
    delete _comp;
    delete _block;
  }

  Block *block() const { return _block; }

  Comparison *comparison() const { return _comp; }

  void accept(Visitor *visitor) override;
};

class While : public ASTNode {
private:
  Comparison *_comp;
  Block *_block;

public:
  While(Comparison *comp, Block *block) {
    _comp = comp;
    _block = block;
  }

  ~While() {
    delete _comp;
    delete _block;
  }

  void accept(Visitor *visitor) override;

  Comparison *comparison() const { return _comp; }

  Block *block() const { return _block; }
};

class Input : public ASTNode {
private:
  Identifier *_ident;

public:
  Input(Identifier *ident) { _ident = ident; }

  ~Input() { delete _ident; }

  void accept(Visitor *visitor) override;

  Identifier *identifer() const { return _ident; }
};

class Goto : public ASTNode {
private:
  Identifier *_ident;

public:
  Goto(Identifier *ident) { _ident = ident; }

  ~Goto() { delete _ident; }

  void accept(Visitor *visitor) override;

  Identifier *identifer() const { return _ident; }
};

class Label : public ASTNode {
private:
  Identifier *_ident;

public:
  Label(Identifier *ident) { _ident = ident; }

  ~Label() { delete _ident; }

  void accept(Visitor *visitor) override;

  Identifier *identifer() const { return _ident; }
};
