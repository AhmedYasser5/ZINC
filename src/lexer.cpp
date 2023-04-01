#include <string.h>
#include <iostream>
#include "../include/lexer.h"

Token::Token(std::string Text,TOKEN_TYPE T){
    text = Text ;
    type = T ;
    }
Lexer::Lexer(std::string s)
{
    source = s + '\n';
    curChar = ' ';
    curPos = -1 ;
    nextChar();
}
 void Lexer::nextChar()
    {
        curPos += 1 ;
        if (curPos >= source.size())
            curChar = '\0'; //EOF - End of file
        else
            curChar = source[curPos];
    }
    char Lexer::Peek()
    {
        if (curPos+1 >= source.size())
            return '\0';
        else
            return source[curPos+1];
    }
    Token Lexer::getToken()
    {
        TOKEN_TYPE t ;
        std::string text ;
        while (curChar == ' ' || curChar =='\r' || curChar == '\t')
            nextChar();
        switch (curChar)
        {
        case '+':
            t = PLUS ;
            text = curChar;
            break;
        case '-':
            t = MINUS;
            text = curChar;
            break;
        case '*':
            t = ASTERISK;
            text = curChar;
            break;
        case '/':
            t = SLASH;
            text = curChar;
            break;
        case '=':
            if (Peek() == '='){
            text = text + curChar + '=' ;
            t = EQEQ ;
            nextChar();
            }
            else{
            text = curChar ;
            t = EQ;
            }
            break;
        case '>':
            if (Peek() == '='){
                text = text + curChar + '=';
                t = GTEQ ;
                nextChar();
            }
            else{
                text = curChar ;
                t = GT ;
            }
            break;
        case '<':
            if(Peek() == '='){
               text = text + curChar + '=';
               t = LTEQ ;
               nextChar();
            }
            else{
                text = curChar ;
                t = LT ;
            }
            break;
        case '!':
            if (Peek() == '='){
                text = text + curChar + '=';
                t = NOTEQ ;
                nextChar();
            }
            else{
                std::cerr<<"Illegal token";
            }
            break;

        NUM_CASE
            t = NUMBER ;
            text = curChar ;
            while (Peek() >= '0' && Peek() <= '9'){
                text = text + Peek();
                nextChar();
            }
            break;
        case '\n':
            t = NEWLINE;
            text = curChar;
            break;
        case '\0':
            t = _EOF;
            text = curChar;
            break;
        default:
            std::cerr<<"unidentified Token";
        };
        nextChar();
        return Token(text,t);
    }