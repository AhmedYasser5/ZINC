#include <iostream>
#include "lexer.h"
#include <string.h>
#include <fstream>
using namespace std;
void Test(string filepath){
  cout<<"Test for  "<<filepath<<endl;
  cout<<"**************************************************"<<endl;
  fstream file ;
  string str ;
  file.open(filepath);
  if (file.is_open()){
  while (file){
  getline(file,str);
  Lexer l (str);
  while (l.curChar != '\0'){
    Token t = l.getToken();
    cout<<t.text<<" -- "<< t.type <<endl;
  }
  }
  file.close();
  }
}
int main(int argc, char **argv) {
  /*
  if (argc < 2) {
    cout << "Wrong number of arguments\n";
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  */
  Test("/home/rana/Desktop/ZINC/src/test.txt");
    return 0;
}
