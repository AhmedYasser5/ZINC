#include "lexer.hpp"
#include "parser.hpp"
#include "emitter.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    ifstream in("test.in");
    string temp, program;
    while (getline(in, temp)) { program += temp + '\n';}

    Lexer lexer(program);
    vector<Token> lexed_program;
    do {lexed_program.push_back(lexer.getToken());} while (lexed_program.back() != _EOF);

    auto parsed_program = Parser::parse(lexed_program.cbegin(), lexed_program.cend());
    if (!parsed_program.index())
    {
        for (auto &error : get<0>(parsed_program))
            cout << error << endl;
        return -1;
    }

    ofstream out("out.cpp");
    CompilerVisitor emitter(out);
    get<1>(parsed_program)->accept(&emitter);
    return 0;
}
