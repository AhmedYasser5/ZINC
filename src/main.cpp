#include "emitter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <BASIC-filepath>" << endl;
    return 1;
  }
  string path = argv[1];
  if (path.size() < 5 || path.substr(path.size() - 5) != ".ZINC") {
    cout << "The provided filename should end in \".ZINC\"" << endl;
    return 2;
  }
  ifstream in(path);
  string temp, program;
  while (getline(in, temp)) {
    program += temp + '\n';
  }

  Lexer lexer(program);
  vector<Token> lexed_program;
  do {
    lexed_program.push_back(lexer.getToken());
  } while (lexed_program.back() != _EOF);

  auto parsed_program =
      Parser::parse(lexed_program.cbegin(), lexed_program.cend());
  if (!parsed_program.index()) {
    for (auto &error : get<0>(parsed_program))
      cout << error << endl;
    return -1;
  }

  ofstream out(path.substr(0, path.size() - 4) + "cpp");
  CompilerVisitor emitter(out);
  get<1>(parsed_program)->accept(&emitter);
  return 0;
}
