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

  virtual void visit(Program *node) = 0;

  virtual void visit(Comparison *node) = 0;

  virtual void visit(Expression *node) = 0;

  virtual void visit(Term *node) = 0;
  
  virtual void visit(Unary *node) = 0;

  virtual void visit(IntLiteral *node) = 0;

  virtual void visit(DoubleLiteral *node) = 0;

  virtual void visit(StringLiteral *node) = 0;

  virtual void visit(Identifer *node) = 0;

  virtual void visit(Declare *node) = 0;

  virtual void visit(Print *node) = 0;
  
  virtual void visit(Assign *node) = 0;
};
