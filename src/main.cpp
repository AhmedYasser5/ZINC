// A generic C++ code for an abstract syntax tree in a source to source compiler
#include "base.hpp"
#include "emitter.hpp"
#include "keywords.hpp"
#include "visitor.hpp"
#include <ostream>

void test() {
  // Create a compiler visitor that prints the target code to std::cout
  CompilerVisitor compiler(std::cout);

  ASTNode *ast1 = new Statement(new Declare(
      new Identifer("int", new Variable("myvar")),
      new BinaryExpr('+', new Variable("num"), new IntLiteral(10))));

  ASTNode *ast2 = new Statement(new Declare(
      new Identifer("int", new Variable("other_var")),
      new BinaryExpr('*', new Variable("myvar"), new IntLiteral(10))));
  ASTNode *ast3 = new Statement(
      new Assign("myvar", new BinaryExpr('*', new Variable("other_var"),
                                         new IntLiteral(15))));
  std::vector<ASTNode *> vec;
  vec.emplace_back(ast1);
  vec.emplace_back(ast2);
  vec.emplace_back(ast3);
  ASTNode *ast = new Block(vec);

  // Traverse the AST using the compiler visitor
  ast->accept(&compiler);

  std::cout << std::endl; // Print a newline

  delete ast;
}

// A main function to test the code
int main() {
  test();
  return 0; // Return 0 from main
}

/*#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Wrong number of arguments\n";
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  return 0;
}*/
