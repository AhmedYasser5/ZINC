#pragma once
#include "base.hpp"
#include "keywords.hpp"
#include "visitor.hpp"
#include <ostream>
#include <stack>
#include <unordered_map>
#include <vector>

// A class for a source to source compiler visitor that generates the target

class CompilerVisitor : public Visitor {
private:
  // The reference to the output stream
  std::ostream &out;
  std::unordered_map<std::string, int> variable_map;
  std::stack<std::string> variable_stack;
  int scope;
  bool first_access;

public:
  // A constructor that takes the output stream as an argument
  CompilerVisitor(std::ostream &out) : out(out), scope(0), first_access(true) {
    variable_map.clear();
    out << "#include <iostream>\nusing namespace std;\nint main()\n{\n";
  }

  ~CompilerVisitor() { out << "return 0;\n}\n"; }

  void visit(Block *node) override {
    int cur_stack_size = variable_stack.size();
    bool local_access = true;
    if (!first_access) {
      out << "{\n";
      scope++;
    } else {
      first_access = false;
      local_access = false;
    }
    std::vector<ASTNode *> vec = node->statements();
    for (auto iter : vec)
      iter->accept(this);
    for (int i = variable_stack.size() - cur_stack_size; i > 0; i--) {
      if (variable_map[variable_stack.top()] == scope) {
        variable_map.erase(variable_stack.top());
        variable_stack.pop();
      }
    }
    if (local_access) {
      out << "}\n";
      scope--;
    }
  }

  void visit(Unary *node) override {
    if (node->op().text == "-")
      out << node->op().text;
    node->primary()->accept(this);
  }

  void visit(Term *node) override {
    node->left()->accept(this);
    out << " " << node->op().text << " ";
    node->right()->accept(this);
  }

  void visit(Expression *node) override {
    node->left()->accept(this);
    out << " " << node->op().text << " ";
    node->right()->accept(this);
  }

  void visit(Comparison *node) override {
    node->left()->accept(this);
    out << " " << node->op().text << " ";
    node->right()->accept(this);
  }

  void visit(DoubleLiteral *node) override { out << node->value(); }

  void visit(StringLiteral *node) override {
    out << "\"" << node->str() << "\"";
  }

  void visit(Identifier *node) override { out << node->ident(); }

  void visit(Print *node) override {
    out << "cout << ";
    node->argument()->accept(this);
    out << " << endl;\n";
  }

  void visit(Let *node) override {
    if (variable_map.find(node->ident()->ident()) == variable_map.end() ||
        variable_map[node->ident()->ident()] > scope) {
      out << "double ";
      variable_stack.push(node->ident()->ident());
      variable_map[node->ident()->ident()] = scope;
    }
    node->ident()->accept(this);
    out << " = ";
    node->expr()->accept(this);
    out << ";\n";
  }

  void visit(If *node) override {
    out << "if (";
    node->comparison()->accept(this);
    out << ")\n";
    node->block()->accept(this);
    if(node->next() != NULL)
        node->next()->accept(this);
  }

  void visit(Elseif *node) override {
    out << "else if (";
    node->comparison()->accept(this);
    out << ")\n";
    node->block()->accept(this);
    if(node->next() != NULL)
        node->next()->accept(this);
  }

  void visit(Else *node) override {
    out << "else\n";
    node->block()->accept(this);
  }

  void visit(While *node) override {
    out << "while (";
    node->comparison()->accept(this);
    out << ")\n";
    node->block()->accept(this);
  }

  void visit(Input *node) override {
    if (variable_map.find(node->identifer()->ident()) == variable_map.end() ||
        variable_map[node->identifer()->ident()] > scope) {
      out << "double ";
      variable_stack.push(node->identifer()->ident());
      variable_map[node->identifer()->ident()] = scope;
      node->identifer()->accept(this);
      out << ";\n";
    }
    out << "cin >> ";
    node->identifer()->accept(this);
    out << ";\n";
  }

  void visit(Goto *node) override {
    out << "goto ";
    node->identifer()->accept(this);
    out << ";\n";
  }

  void visit(Label *node) override {
    node->identifer()->accept(this);
    out << ":\n";
  }
};
