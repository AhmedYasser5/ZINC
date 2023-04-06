#pragma once
#include "base.hpp"
#include "keywords.hpp"
#include "visitor.hpp"
#include <ostream>
#include <vector>

// A class for a source to source compiler visitor that generates the target
// code
class CompilerVisitor : public Visitor {
private:
  // The reference to the output stream
  std::ostream &out;

public:
  // A constructor that takes the output stream as an argument
  CompilerVisitor(std::ostream &out) : out(out) {
    out << "#include <iostream>\n"
        << "using namespace std;\n"
        << "int main() {\n";
  }

  ~CompilerVisitor() { out << "return 0;\n}\n"; }

  // A function to visit a binary expression node
  void visit(BinaryExpr *node) override {
    out << "("; // Print an opening parenthesis

    node->getLeft()->accept(this); // Visit the left operand

    out << " " << node->getOp() << " "; // Print the operator

    node->getRight()->accept(this); // Visit the right operand

    out << ")"; // Print a closing parenthesis
  }

  // A function to visit an integer literal node
  void visit(IntLiteral *node) override {
    out << node->getValue(); // Print the value
  }

  void visit(Identifer *node) override {
    out << node->getIdent() << " ";
    node->getVariable()->accept(this);
  }

  void visit(Variable *node) override { out << node->getName(); }

  void visit(Declare *node) override {
    node->getIdent()->accept(this);
    out << " = ";
    node->getExpr()->accept(this);
  }

  void visit(Assign *node) override {
    out << node->getVariable() << " = ";
    node->getExpr()->accept(this);
  }

  void visit(Statement *node) override {
    node->getStatement()->accept(this);
    out << ";\n";
  }

  void visit(Block *node) override {
    std::vector<ASTNode *> vec = node->getStatements();
    for (auto it : vec)
      it->accept(this);
  }
};
