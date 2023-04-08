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
 * Identifer
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
    _statements.clear();
  }

  void accept(Visitor *visitor) override;

  std::vector<ASTNode *> statements() const { return _statements; }
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

class Term : public ASTNode {
private:
  char _op;
  Unary *_left;
  Unary *_right;

public:
  Term(Unary *left, char op, Unary *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Term() {
    delete _left;
    delete _right;
  }

  Unary *left() const { return _left; }

  Unary *right() const { return _right; }

  char op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Expression : public ASTNode {
private:
  char _op;
  Unary *_left;
  Unary *_right;

public:
  Expression(Unary *left, char op, Unary *right) {
    this->_left = left;
    this->_op = op;
    this->_right = right;
  }

  ~Expression() {
    delete _left;
    delete _right;
  }

  Unary *left() const { return _left; }

  Unary *right() const { return _right; }

  char op() const { return _op; }

  void accept(Visitor *visitor) override;
};

class Comparison : public ASTNode {
private:
  Token _op;
  Expression *_left;
  Expression *_right;

public:
  Comparison(Expression *left, Token op, Expression *right) {
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

class Let : public ASTNode {
private:
  ASTNode *_ident;
  ASTNode *_expr;
  Token _data_type;

public:
  Let(ASTNode *identifer, ASTNode *expr, Token data_type) {
    _ident = identifer;
    _expr = expr;
    _data_type = data_type;
  }

  ~Let() {
    delete _expr;
    delete _ident;
  }

  void accept(Visitor *visitor) override;

  ASTNode *ident() const { return _ident; }

  ASTNode *expr() const { return _expr; }

  Token data_type() const { return _data_type; }
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
    Comparison* _comp;
    Block *_block;

  public:
    While(Comparison *comp, Block *block)
    {
      _comp = comp;
      _block = block;
    }

    ~While() {
      delete _comp;
      delete _block;
    }
    
  void accept(Visitor *visitor) override;

  Comparison* comparison() const { return _comp; }

  Block *block() const { return _block; }
};

class Input : public ASTNode {
  private:
    Identifer *_ident;

  public:
    Input(Identifer *ident) { _ident = ident; }

    ~Input() { delete _ident; }

    void accept(Visitor* visitor) override;

    Identifer *identifer() const { return _ident; }
};

class Goto : public ASTNode {
  private:
    Identifer *_ident;

  public:
    Goto(Identifer *ident) { _ident = ident; }

    ~Goto() { delete _ident; }

    void accept(Visitor* visitor) override;

    Identifer *identifer() const { return _ident; }
};

class Label : public ASTNode {
  private:
    Identifer *_ident;

  public:
    Label(Identifer *ident) { _ident = ident; }

    ~Label() { delete _ident; }

    void accept(Visitor* visitor) override;

    Identifer *identifer() const { return _ident; }
};
