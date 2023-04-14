#include <iostream>
#include "lexer.h"
#include <string.h>
#include <fstream>
using namespace std;
void Test(){
  fstream file ;
  string str ;
  file.open("./src/test.txt",ios::in);
  while (!file.eof()){
  getline(file,str);
  Lexer l (str);
  while (l.curChar != '\0'){
    Token t = l.getToken();
    cout<<t.text<<" -- "<< t.type <<endl;
  }
  }
  file.close();
}
int main(int argc, char **argv) {
    Test();
    return 0;
}
