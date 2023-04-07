#include <iostream>
#include "lexer.h"
#include <string.h>
using namespace std;

int main(int argc, char **argv) {
  /*
  if (argc < 2) {
    cout << "Wrong number of arguments\n";
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  */
  cout<<"start"<<endl;
  Lexer l("IF+-123 foo*THEN/");
  while (l.curChar != '\0'){
    Token t = l.getToken();
               cout<<t.text<<" -- "<< t.type <<endl;
  }
    return 0;
}
