#pragma once
#include "base.hpp"
#include <string>
#include <vector>

class Block : public ASTNode {
private:
  std::vector<ASTNode *> statements;

public:
  Block(std::vector<ASTNode *> vec) { statements = vec; }

  ~Block() {
    for (auto it : statements)
      delete it;
    statements.clear();
  }

  void accept(Visitor *visitor) override;

  std::vector<ASTNode *> getStatements() const { return statements; }
};

// A class for binary expressions
class BinaryExpr : public ASTNode {
private:
  char op;        // The operator
  ASTNode *left;  // The pointer to the left operand
  ASTNode *right; // The pointer to the right operand

public:
  // A constructor that takes the operator and the operands as arguments
  BinaryExpr(char op, ASTNode *left, ASTNode *right) {
    this->op = op;
    this->left = left;
    this->right = right;
  }

  // A destructor that deletes the operands
  ~BinaryExpr() {
    delete left;
    delete right;
  }

  // A function to accept a visitor
  void accept(Visitor *visitor) override;

  // A function to get the operator
  char getOp() const { return op; }

  // A function to get the left operand
  ASTNode *getLeft() const { return left; }

  // A function to get the right operand
  ASTNode *getRight() const { return right; }
};

// A class for integer literals
class IntLiteral : public ASTNode {
private:
  // The value
  int value;

public:
  // A constructor that takes the value as an argument
  IntLiteral(int value) { this->value = value; }

  // A function to accept a visitor
  void accept(Visitor *visitor) override;

  // A function to get the value
  int getValue() const { return value; }
};

class Identifer : public ASTNode {
private:
  std::string identifer;
  ASTNode *variable;

public:
  Identifer(std::string value, ASTNode *variable) {
    this->identifer = value;
    this->variable = variable;
  }

  void accept(Visitor *visitor) override;

  std::string getIdent() const { return identifer; }

  ASTNode *getVariable() const { return variable; }

  ~Identifer() { delete variable; }
};

class Variable : public ASTNode {
private:
  std::string name;

public:
  Variable(std::string name) { this->name = name; }

  void accept(Visitor *visitor) override;

  std::string getName() const { return name; }
};

class Declare : public ASTNode {
private:
  ASTNode *identifer;
  ASTNode *expr;

public:
  Declare(ASTNode *identifer, ASTNode *expr) {
    this->identifer = identifer;
    this->expr = expr;
  }

  ~Declare() {
    delete expr;
    delete identifer;
  }

  void accept(Visitor *visitor) override;

  ASTNode *getIdent() const { return identifer; }

  ASTNode *getExpr() const { return expr; }
};

class Assign : public ASTNode {
private:
  ASTNode *expr;
  std::string variable;

public:
  Assign(std::string var_name, ASTNode *expression) {
    variable = var_name;
    expr = expression;
  }

  ~Assign() { delete expr; }

  void accept(Visitor *visitor) override;

  std::string getVariable() const { return variable; }

  ASTNode *getExpr() const { return expr; }
};

class Statement : public ASTNode {
private:
  ASTNode *statement;

public:
  Statement(ASTNode *statement) { this->statement = statement; }

  ~Statement() { delete statement; }

  void accept(Visitor *visitor) override;

  ASTNode *getStatement() const { return statement; }
};
