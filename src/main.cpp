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
a = "EQUAL";
else if (s ==PLUS)
a = "PLUS";
else if (s ==MINUS)
a = "MINUS";
else if (s ==ASTERISK)
a = "ASTERISK";
else if (s ==SLASH)
a = "SLASH";
else if (s ==EQEQ )
a = "EQUAL EQUAL";
else if (s ==NOTEQ)
a = "NOT EQUAL";
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
  string program,temp;
  file.open("./src/test.txt",ios::in);
  while (getline(file,temp)){
    program = program + temp + '\n';
  }
  Lexer l (program);
  while (l.curChar != '\0'){
    cout<<"Line "<<l.curLine<<endl;
    Token t = l.getToken();
    cout<<t.text<<"   "<< typeToString(t.type) << " At Line : "<<t.line<<endl;
  }
  file.close();
}
int main(int argc, char **argv) {
  try{
    Test();
  }
  catch(int &e){
    cout<<"Program terminated due to Error..."<<endl;
    return 1 ;
  }
    return 0;
}
