#include "parser.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

using token_iterator = std::vector<Token>::const_iterator;
using namespace std;

static Identifier *identifier(token_iterator &begin, const token_iterator &end);
static StringLiteral *string_literal(token_iterator &begin,
                                     const token_iterator &end);
static DoubleLiteral *double_literal(token_iterator &begin,
                                     const token_iterator &end);
static Comparison *comparison(token_iterator &begin, const token_iterator &end);
static MathNode *expression(token_iterator &begin, const token_iterator &end);
static MathNode *term(token_iterator &begin, const token_iterator &end);
static Unary *unary(token_iterator &begin, const token_iterator &end);
static Primary *primary(token_iterator &begin, const token_iterator &end);
static Print *print_statement(token_iterator &begin, const token_iterator &end);
static If *if_statement(token_iterator &begin, const token_iterator &end);
static While *while_statement(token_iterator &begin, const token_iterator &end);
static Label *label_statement(token_iterator &begin, const token_iterator &end);
static Goto *goto_statement(token_iterator &begin, const token_iterator &end);
static Let *let_statement(token_iterator &begin, const token_iterator &end);
static Input *input_statement(token_iterator &begin, const token_iterator &end);
static ASTNode *statement(token_iterator &begin, const token_iterator &end);
static Block *block(token_iterator &begin, const token_iterator &end);
static Identifier *identifier(token_iterator &begin, const token_iterator &end);
static bool match(token_iterator &begin, const token_iterator &end,
                  initializer_list<TOKEN_TYPE> types);
static bool consume(token_iterator &begin, const token_iterator &end,
                    TOKEN_TYPE type);

bool consume(token_iterator &begin, const token_iterator &end,
             TOKEN_TYPE type) {
  if (!match(begin, end, {type})) {
    return false;
  }
  while (match(begin, end, {type}))
    ;
  return true;
}

bool match(token_iterator &begin, const token_iterator &end,
           initializer_list<TOKEN_TYPE> types) {
  if (begin == end) {
    return false;
  }
  for (auto &type : types)
    if (begin->type == type) {
      ++begin;
      return true;
    }
  return false;
}

StringLiteral *string_literal(token_iterator &begin,
                              const token_iterator &end) {
  if (begin == end || begin->type != STRING) {
    return nullptr;
  }
  return new StringLiteral((begin++)->text);
}

Identifier *identifier(token_iterator &begin, const token_iterator &end) {
  if (begin == end || begin->type != IDENT) {
    return nullptr;
  }
  return new Identifier((begin++)->text);
}

DoubleLiteral *double_literal(token_iterator &begin,
                              const token_iterator &end) {
  if (begin == end || begin->type != NUMBER) {
    return nullptr;
  }
  return new DoubleLiteral(stod((begin++)->text));
}

Comparison *comparison(token_iterator &begin, const token_iterator &end) {
  unique_ptr<MathNode> first_expr(expression(begin, end));
  if (first_expr == nullptr ||
      !match(begin, end, {EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) {
    return nullptr;
  }
  Token op = begin[-1];
  MathNode *second_expr = expression(begin, end);
  if (second_expr == nullptr) {
    return nullptr;
  }
  auto cmp = make_unique<Comparison>(first_expr.release(), op, second_expr);
  while (match(begin, end, {EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) {
    Token op = begin[-1];
    MathNode *second_expr = expression(begin, end);
    if (second_expr == nullptr) {
      return nullptr;
    }
    cmp.reset(new Comparison(cmp.release(), op, second_expr));
  }
  return cmp.release();
}

MathNode *expression(token_iterator &begin, const token_iterator &end) {
  unique_ptr<MathNode> first_term(term(begin, end));
  if (first_term == nullptr) {
    return nullptr;
  }
  while (match(begin, end, {PLUS, MINUS})) {
    Token op = begin[-1];
    MathNode *second_term = term(begin, end);
    if (second_term == nullptr) {
      return nullptr;
    }
    first_term.reset(new Expression(first_term.release(), op, second_term));
  }
  return first_term.release();
}

MathNode *term(token_iterator &begin, const token_iterator &end) {
  unique_ptr<MathNode> first_unary(unary(begin, end));
  if (first_unary == nullptr) {
    return nullptr;
  }
  while (match(begin, end, {ASTERISK, SLASH})) {
    Token op = begin[-1];
    MathNode *second_unary = unary(begin, end);
    if (second_unary == nullptr) {
      return nullptr;
    }
    first_unary.reset(new Term(first_unary.release(), op, second_unary));
  }
  return first_unary.release();
}

Unary *unary(token_iterator &begin, const token_iterator &end) {
  Token sign("+", PLUS);
  if (match(begin, end, {PLUS, MINUS})) {
    sign = begin[-1];
  }
  Primary *num = primary(begin, end);
  if (num == nullptr) {
    return nullptr;
  }
  return new Unary(sign, num);
}

Primary *primary(token_iterator &begin, const token_iterator &end) {
  switch (begin->type) {
  case IDENT:
    return identifier(begin, end);
  case NUMBER:
    return double_literal(begin, end);
  default:;
  };
  return nullptr;
}

Print *print_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<ASTNode> to_be_printed;
  if (begin->type == STRING) {
    to_be_printed.reset(string_literal(begin, end));
  } else {
    to_be_printed.reset(expression(begin, end));
  }
  if (to_be_printed == nullptr) {
    return nullptr;
  }
  return new Print(to_be_printed.release());
}

If *if_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Comparison> cmp(comparison(begin, end));
  if (cmp == nullptr || !match(begin, end, {THEN}) ||
      !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  unique_ptr<Block> scope(block(++begin, end));
  if (!match(begin, end, {ENDIF}) || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new If(cmp.release(), scope.release());
}

While *while_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Comparison> cmp(comparison(begin, end));
  if (cmp == nullptr || !match(begin, end, {REPEAT}) ||
      !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  unique_ptr<Block> scope(block(++begin, end));
  if (!match(begin, end, {ENDWHILE}) || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new While(cmp.release(), scope.release());
}

Label *label_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Identifier> ident(identifier(begin, end));
  if (ident == nullptr || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new Label(ident.release());
}

Goto *goto_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Identifier> ident(identifier(begin, end));
  if (ident == nullptr || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new Goto(ident.release());
}

Let *let_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Identifier> ident(identifier(begin, end));
  if (ident == nullptr || !match(begin, end, {EQ})) {
    return nullptr;
  }
  unique_ptr<MathNode> expr(expression(begin, end));
  if (expr == nullptr || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new Let(ident.release(), expr.release());
}

Input *input_statement(token_iterator &begin, const token_iterator &end) {
  unique_ptr<Identifier> ident(identifier(begin, end));
  if (ident == nullptr || !consume(begin, end, NEWLINE)) {
    return nullptr;
  }
  return new Input(ident.release());
}

ASTNode *statement(token_iterator &begin, const token_iterator &end) {
  static unordered_map<TOKEN_TYPE,
                       function<ASTNode *(token_iterator &, token_iterator)>>
      statement_functions = {{PRINT, print_statement}, {IF, if_statement},
                             {WHILE, while_statement}, {LABEL, label_statement},
                             {GOTO, goto_statement},   {LET, let_statement},
                             {INPUT, input_statement}};
  if (begin == end) {
    return nullptr;
  }
  auto statment_parser = statement_functions.find(begin->type);
  if (statment_parser == statement_functions.end()) {
    return nullptr;
  }
  return statment_parser->second(++begin, end);
}

Block *block(token_iterator &begin, const token_iterator &end) {
  vector<ASTNode *> statements;
  do {
    statements.push_back(statement(begin, end));
  } while (statements.back() != nullptr);
  statements.pop_back();
  if (statements.empty()) {
    return nullptr;
  }
  return new Block(std::move(statements));
}

unique_ptr<ASTNode> parse(token_iterator begin, token_iterator end) {
  unique_ptr<ASTNode> program_tree(block(begin, end));
  if (program_tree == nullptr || begin != end) {
    return nullptr;
  }
  return program_tree;
}
