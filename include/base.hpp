#pragma once
// An abstract base class for AST nodes
class ASTNode {
public:
  // A virtual destructor
  virtual ~ASTNode() {}

  // A pure virtual function to accept a visitor
  virtual void accept(class Visitor *visitor) = 0;
};

class MathNode : ASTNode{
public:
  // A virtual destructor
  virtual ~MathNode() {}

  // A pure virtual function to accept a visitor
  virtual void accept(class Visitor *visitor) = 0;
};

class Primary : ASTNode{
public:
  // A virtual destructor
  virtual ~Primary() {}

  // A pure virtual function to accept a visitor
  virtual void accept(class Visitor *visitor) = 0;
};
