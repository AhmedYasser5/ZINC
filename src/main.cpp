#include <iostream>
#include "lexer.h"
#include <string.h>
#include <fstream>
using namespace std;
string typeToString(TOKEN_TYPE s){
  string a ;
if(s== LABEL)
a = "LABEL";
else if(s== GOTO)
a = "GOTO";
else if(s== INPUT)
a = "INPUT";
else if(s== LET)
a = "LET";
else if(s== IF)
a = "IF";
else if(s== THEN)
a = "THEN";
else if(s== ENDIF)
a = "ENDIF";
else if(s== WHILE)
a = "WHILE";
else if(s== REPEAT)
a = "REPEAT";
else if(s== ENDWHILE)
a = "ENDWHILE";
else if (s == PRINT)
a = "PRINT";
else if (s == NEWLINE)
a = "NEWLINE";
else if (s == NONE)
a = "NONE";
else if (s ==_EOF)
a = "END OF FILE";
else if (s ==NUMBER )
a = "NUMBER";
else if (s ==STRING)
a = "STRING";
else if (s ==EQ)
a = "EQ";
else if (s ==PLUS)
a = "PLUS";
else if (s ==MINUS)
a = "MINUS";
else if (s ==ASTERISK)
a = "ASTERISK";
else if (s ==SLASH)
a = "SLASH";
else if (s ==EQEQ )
a = "EQEQ";
else if (s ==NOTEQ)
a = "NOTEQ";
else if (s ==LT )
a = "LESS THAT";
else if (s ==LTEQ)
a = "LESS THAN EQUAL";
else if (s ==GT)
a = "GREATER THAN";
else if (s ==GTEQ)
a = "GREATER THAN EQUAL";
else
a = "IDENT";
return a ;
}
void Test(){
  fstream file ;
  string str ;
  file.open("./src/test.txt",ios::in);
  while (!file.eof()){
  getline(file,str);
  Lexer l (str);
  while (l.curChar != '\0'){
    Token t = l.getToken();
    cout<<t.text<<" -- "<< typeToString(t.type) <<endl;
  }
  }
  file.close();
}
int main(int argc, char **argv) {
    Test();
    return 0;
}
