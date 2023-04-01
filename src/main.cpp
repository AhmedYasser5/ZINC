#include <iostream>
#include "lexer.h"
using namespace std;

int main(int argc, char **argv) {
  /*
  if (argc < 2) {
    cout << "Wrong number of arguments\n";
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  */
  Lexer l("1+2+3+4=16");
  while (l.Peek() != '\0'){
    Token t = l.getToken();
    cout<<t.text<<" "<< t.type <<endl;
  }
  return 0;
}
