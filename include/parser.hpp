#pragma once

#include "keywords.hpp"
#include "lexer.hpp"
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace Parser {

using std::get;
using std::initializer_list;
using std::make_unique;
using std::queue;
using std::stack;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;
using std::variant;
using std::vector;

template <typename TokenIterator> class Parser {
public:
  unique_ptr<Block> program(TokenIterator cbegin, TokenIterator cend) {
    this->cbegin = cbegin;
    this->cend = cend;
    _errors.clear();
    labels.clear();
    gotoed.clear();
    unique_ptr<Block> program_tree(block());
    if (match({_EOF}) == NONE) {
      report_error("an EoF");
    }
    for (auto &gotoed : gotoed) {
      if (labels.find(gotoed) == labels.end()) {
        _errors.push_back("Label \"");
        _errors.back() += gotoed;
        _errors.back() += "\" was used without being declared";
      }
    }
    if (!_errors.empty()) {
      return nullptr;
    }
    return program_tree;
  }

  vector<string> errors() const { return _errors; }

private:
#define return_if_error(x)                                                     \
  do {                                                                         \
    if (x == nullptr) {                                                        \
      return nullptr;                                                          \
    }                                                                          \
  } while (0)

  bool consume(TOKEN_TYPE type) {
    if (match({type}) == NONE) {
      return false;
    }
    while (match({type}) != NONE)
      ;
    return true;
  }

  bool consume_till(TOKEN_TYPE type, bool included = true) {
    while (true) {
      auto token = peek();
      if (token == NONE || token == _EOF || token == type) {
        break;
      }
      match();
    }
    return (included ? match({type}) : peek({type})) == type;
  }

  Token match(initializer_list<TOKEN_TYPE> types = {}, bool peek = false) {
    if (cbegin == cend) {
      return {};
    }
    auto consume = [&]() -> Token {
      auto token = *cbegin;
      if (!peek) {
        ++cbegin;
      }
      return token;
    };
    if (types.size() == 0) {
      return consume();
    }
    for (auto &type : types) {
      if (cbegin->type == type) {
        return consume();
      }
    }
    return {};
  }

  Token peek(initializer_list<TOKEN_TYPE> types = {}) {
    return match(types, true);
  }

  void report_error(string_view expected) {
    _errors.push_back("Expected ");
    _errors.back() += expected;
    _errors.back() += ", but found ";
    if (cbegin == cend) {
      _errors.back() += "an unexpected EoF";
    } else {
      _errors.back() += "\"" + cbegin->text + "\"";
    }
  }

  StringLiteral *string_literal() {
    Token str;
    if ((str = match({STRING})) == NONE) {
      report_error("a string literal");
      return nullptr;
    }
    return new StringLiteral(str.text);
  }

  Identifier *identifier() {
    Token ident;
    if ((ident = match({IDENT})) == NONE) {
      report_error("an identifier");
      return nullptr;
    }
    return new Identifier(ident.text);
  }

  DoubleLiteral *double_literal() {
    Token num;
    if ((num = match({NUMBER})) == NONE) {
      report_error("a number literal");
      return nullptr;
    }
    return new DoubleLiteral(stod(num.text));
  }

  Comparison *comparison() {
    unique_ptr<MathNode> first_expr(expression());
    return_if_error(first_expr);
    Token op;
    if ((op = match({EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) == NONE) {
      report_error("a comparison operator");
      return nullptr;
    }
    MathNode *second_expr = expression();
    return_if_error(second_expr);
    auto cmp = make_unique<Comparison>(first_expr.release(), op, second_expr);
    while ((op = match({EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) != NONE) {
      MathNode *second_expr = expression();
      return_if_error(second_expr);
      cmp.reset(new Comparison(cmp.release(), op, second_expr));
    }
    return cmp.release();
  }

  MathNode *expression() {
    unique_ptr<MathNode> first_term(term());
    return_if_error(first_term);
    Token op;
    while ((op = match({PLUS, MINUS})) != NONE) {
      MathNode *second_term = term();
      return_if_error(second_term);
      first_term.reset(new Expression(first_term.release(), op, second_term));
    }
    return first_term.release();
  }

  MathNode *term() {
    unique_ptr<MathNode> first_unary(unary());
    return_if_error(first_unary);
    Token op;
    while ((op = match({ASTERISK, SLASH})) != NONE) {
      Unary *second_unary = unary();
      return_if_error(second_unary);
      first_unary.reset(new Term(first_unary.release(), op, second_unary));
    }
    return first_unary.release();
  }

  Unary *unary() {
    Token sign = match({PLUS, MINUS});
    if (sign == NONE) {
      sign = Token("+", PLUS);
    }
    Primary *num = primary();
    return_if_error(num);
    return new Unary(sign, num);
  }

  Primary *primary() {
    switch (peek().type) {
    case IDENT:
      break;
    case NUMBER:
      return double_literal();
    default:
      report_error("a primary");
      return nullptr;
    }
    unique_ptr<Identifier> ident(identifier());
    if (variables.find(ident->ident()) == variables.end()) {
      _errors.push_back("Variable \"");
      _errors.back() += ident->ident();
      _errors.back() += "\" used without being defined";
      return nullptr;
    }
    return ident.release();
  }

  ASTNode *print_statement() {
    ASTNode *to_be_printed = nullptr;
    switch (peek().type) {
    case STRING:
      to_be_printed = string_literal();
      break;
    case NONE:
      report_error("a string or an expression in a PRINT statement");
      break;
    default:
      to_be_printed = expression();
    }
    if (!consume_till(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(to_be_printed);
    return new Print(to_be_printed);
  }

  ASTNode *if_statement() {
    queue<unique_ptr<Subif>> ifelses;
    do {
      unique_ptr<Comparison> cmp(comparison());
      if (!consume_till(THEN)) {
        report_error("the keyword \"THEN\"");
        return nullptr;
      }
      if (!consume(NEWLINE)) {
        report_error("an EoL character");
        return nullptr;
      }
      unique_ptr<Block> scope(block());
      if (cmp == nullptr || scope == nullptr) {
        ifelses.emplace(nullptr);
      } else if (ifelses.empty()) {
        ifelses.emplace(new If(cmp.release(), scope.release()));
      } else {
        ifelses.emplace(new Elseif(cmp.release(), scope.release()));
      }
    } while (match({ELSEIF}) != NONE);
    if (match({ELSE}) != NONE) {
      if (!consume(NEWLINE)) {
        report_error("an EoL character");
        return nullptr;
      }
      ifelses.emplace(new Else(block()));
    }
    if (!consume_till(ENDIF)) {
      report_error("the keyword \"ENDIF\"");
      return nullptr;
    }
    if (!consume(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(ifelses.front());
    unique_ptr<If> whole_if(static_cast<If *>(ifelses.front().release()));
    ifelses.pop();
    Subif *last = whole_if.get();
    while (!ifelses.empty()) {
      return_if_error(ifelses.front());
      last->next(ifelses.front().release());
      ifelses.pop();
      last = last->next();
    }
    return whole_if.release();
  }

  ASTNode *while_statement() {
    unique_ptr<Comparison> cmp(comparison());
    if (!consume_till(REPEAT)) {
      report_error("the keyword \"REPEAT\"");
      return nullptr;
    }
    if (!consume(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    unique_ptr<Block> scope(block());
    if (!consume_till(ENDWHILE)) {
      report_error("the keyword \"ENDWHILE\"");
      return nullptr;
    }
    if (!consume(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(cmp);
    return_if_error(scope);
    return new While(cmp.release(), scope.release());
  }

  ASTNode *label_statement() {
    unique_ptr<Identifier> ident(identifier());
    if (!consume_till(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(ident);
    labels.insert(ident->ident());
    return new Label(ident.release());
  }

  ASTNode *goto_statement() {
    unique_ptr<Identifier> ident(identifier());
    if (!consume_till(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(ident);
    gotoed.insert(ident->ident());
    return new Goto(ident.release());
  }

  void create_variable(const string &variable) {
    if (variables.find(variable) == variables.end()) {
      variables.insert(variable);
      scoped_variables.top().push_back(variable);
    }
  }

  ASTNode *let_statement() {
    unique_ptr<Identifier> ident(identifier());
    if (!consume_till(EQ)) {
      report_error("an '=' operator in a LET statement");
      return nullptr;
    }
    unique_ptr<MathNode> expr(expression());
    if (!consume_till(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(ident);
    create_variable(ident->ident());
    return_if_error(expr);
    return new Let(ident.release(), expr.release());
  }

  ASTNode *input_statement() {
    unique_ptr<Identifier> ident(identifier());
    if (!consume_till(NEWLINE)) {
      report_error("an EoL character");
      return nullptr;
    }
    return_if_error(ident);
    create_variable(ident->ident());
    return new Input(ident.release());
  }

  variant<bool, ASTNode *> statement() {
    static const unordered_map<TOKEN_TYPE,
                               ASTNode *(Parser<TokenIterator>::*)()>
        statement_functions{{PRINT, &Parser<TokenIterator>::print_statement},
                            {IF, &Parser<TokenIterator>::if_statement},
                            {WHILE, &Parser<TokenIterator>::while_statement},
                            {LABEL, &Parser<TokenIterator>::label_statement},
                            {GOTO, &Parser<TokenIterator>::goto_statement},
                            {LET, &Parser<TokenIterator>::let_statement},
                            {INPUT, &Parser<TokenIterator>::input_statement}};
    consume(NEWLINE);
    auto statment_parser = statement_functions.find(peek().type);
    if (statment_parser == statement_functions.cend()) {
      return true;
    }
    match();
    return (this->*(statment_parser->second))();
  }

  Block *block() {
    scoped_variables.emplace();
    vector<ASTNode *> statements;
    while (true) {
      auto result = statement();
      // A statement was skipped
      if (!result.index()) {
        break;
      }
      // Extract the parsed statement
      unique_ptr<ASTNode> stmt(get<1>(result));
      if (stmt != nullptr) {
        statements.push_back(stmt.release());
      }
    }
    for (auto &variable : scoped_variables.top()) {
      variables.erase(variable);
    }
    scoped_variables.pop();
    return new Block(std::move(statements));
  }
#undef return_if_error

  TokenIterator cbegin;
  TokenIterator cend;
  vector<string> _errors;
  unordered_set<string_view> variables;
  unordered_set<string_view> labels;
  unordered_set<string_view> gotoed;
  stack<vector<string_view>> scoped_variables;
};

template <typename TokenIterator>
variant<vector<string>, unique_ptr<Block>> parse(TokenIterator cbegin,
                                                 TokenIterator cend) {
  Parser<TokenIterator> parser;
  unique_ptr<Block> program_tree(parser.program(cbegin, cend));
  if (program_tree == nullptr) {
    return parser.errors();
  }
  return program_tree;
}

} // namespace Parser
