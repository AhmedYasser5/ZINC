#pragma once
#include "base.hpp"
#include "keywords.hpp"
#include <iostream>

// An abstract base class for visitors
class Visitor {
public:
  // A virtual destructor
  virtual ~Visitor() {}

  // A pure virtual function to visit a binary expression node
  virtual void visit(BinaryExpr *node) = 0;

  // A pure virtual function to visit an integer literal node
  virtual void visit(IntLiteral *node) = 0;

  // A pure virtual function to visit the identifer (int) node
  virtual void visit(Identifer *node) = 0;

  // A pure virtual function to visit the variable (int var) node
  virtual void visit(Variable *node) = 0;

  // A pure virtual function to visit the declaration node (int a = 5;)
  virtual void visit(Declare *node) = 0;

  virtual void visit(Assign *node) = 0;

  // A pure virtual function to visit the Statement (start statement and finish
  // with ;)
  virtual void visit(Statement *node) = 0;

  virtual void visit(Block *node) = 0;
};
