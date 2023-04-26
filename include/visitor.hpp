#pragma once
#include "base.hpp"
#include "keywords.hpp"
#include <iostream>

// An abstract base class for visitors
class Visitor {
public:
  // A virtual destructor
  virtual ~Visitor() {}

  virtual void visit(Block *node) = 0;

  virtual void visit(Comparison *node) = 0;

  virtual void visit(Expression *node) = 0;

  virtual void visit(Term *node) = 0;

  virtual void visit(Unary *node) = 0;

  virtual void visit(DoubleLiteral *node) = 0;

  virtual void visit(StringLiteral *node) = 0;

  virtual void visit(Identifier *node) = 0;

  virtual void visit(Let *node) = 0;

  virtual void visit(Print *node) = 0;

  virtual void visit(Input *node) = 0;

  virtual void visit(If *node) = 0;

  virtual void visit(Elseif *node) = 0;

  virtual void visit(Else *node) = 0;

  virtual void visit(While *node) = 0;

  virtual void visit(Goto *node) = 0;

  virtual void visit(Label *node) = 0;
};
