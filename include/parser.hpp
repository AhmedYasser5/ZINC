#pragma once
#include "keywords.hpp"
#include "lexer.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Parser {

using std::function;
using std::get;
using std::initializer_list;
using std::make_unique;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;
template <typename T> using Result = std::variant<std::string, T>;
#define Ok(x) (get<1>((x)))
#define Error(x) (get<0>((x)))
#define return_if_error(x)                                                     \
  do {                                                                         \
    if (!x.index()) {                                                          \
      return Error(x);                                                         \
    }                                                                          \
  } while (0)
#define extract_or_return(x, y)                                                \
  do {                                                                         \
    auto z = (y);                                                              \
    return_if_error(z);                                                        \
    unique_ptr<typename std::remove_pointer<                                   \
        typename std::remove_reference<decltype(Ok(z))>::type>::type>          \
        zz(Ok(z));                                                             \
    x = std::move(zz);                                                         \
  } while (0)

template <typename TokenIterator> class Parser {
public:
  template <typename Iterator>
  friend Result<unique_ptr<ASTNode>> parse(Iterator cbegin, Iterator cend);

  Parser() = delete;
  ~Parser() = delete;

private:
  static bool consume(TokenIterator &cbegin, const TokenIterator &cend,
                      TOKEN_TYPE type) {
    if (match(cbegin, cend, {type}) == NONE) {
      return false;
    }
    while (match(cbegin, cend, {type}) != NONE)
      ;
    return true;
  }

  static Token match(TokenIterator &cbegin, const TokenIterator &cend,
                     initializer_list<TOKEN_TYPE> types) {
    if (cbegin == cend) {
      return Token("", NONE);
    }
    for (auto &type : types)
      if (cbegin->type == type) {
        return *(cbegin++);
      }
    return Token("", NONE);
  }

  static Result<StringLiteral *> string_literal(TokenIterator &cbegin,
                                                const TokenIterator &cend) {
    if (cbegin == cend) {
      return "Unexpected EoF";
    }
    if (cbegin->type != STRING) {
      return "Expected a string literal, but found \"" + cbegin->text + "\"";
    }
    return new StringLiteral((cbegin++)->text);
  }

  static Result<Identifier *> identifier(TokenIterator &cbegin,
                                         const TokenIterator &cend) {
    if (cbegin == cend) {
      return "Unexpected EoF";
    }
    if (cbegin->type != IDENT) {
      return "Expected an identifier, but found \"" + cbegin->text + "\"";
    }
    return new Identifier((cbegin++)->text);
  }

  static Result<DoubleLiteral *> double_literal(TokenIterator &cbegin,
                                                const TokenIterator &cend) {
    if (cbegin == cend) {
      return "Unexpected EoF";
    }
    if (cbegin->type != NUMBER) {
      return "Expected a number literal, but found \"" + cbegin->text + "\"";
    }
    return new DoubleLiteral(stod((cbegin++)->text));
  }

  static Result<Comparison *> comparison(TokenIterator &cbegin,
                                         const TokenIterator &cend) {
    Result<unique_ptr<MathNode>> first_expr;
    extract_or_return(first_expr, expression(cbegin, cend));
    Token op;
    if ((op = match(cbegin, cend, {EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) == NONE) {
      return "Expected a comparison operator, but found \"" + cbegin->text +
             "\"";
    }
    Result<MathNode *> second_expr = expression(cbegin, cend);
    return_if_error(second_expr);
    auto cmp =
        make_unique<Comparison>(Ok(first_expr).release(), op, Ok(second_expr));
    while ((op = match(cbegin, cend, {EQEQ, NOTEQ, GT, GTEQ, LT, LTEQ})) !=
           NONE) {
      Result<MathNode *> second_expr = expression(cbegin, cend);
      return_if_error(second_expr);
      cmp.reset(new Comparison(cmp.release(), op, Ok(second_expr)));
    }
    return cmp.release();
  }

  static Result<MathNode *> expression(TokenIterator &cbegin,
                                       const TokenIterator &cend) {
    Result<unique_ptr<MathNode>> first_term;
    extract_or_return(first_term, term(cbegin, cend));
    Token op;
    while ((op = match(cbegin, cend, {PLUS, MINUS})) != NONE) {
      Result<MathNode *> second_term = term(cbegin, cend);
      return_if_error(second_term);
      Ok(first_term)
          .reset(new Expression(Ok(first_term).release(), op, Ok(second_term)));
    }
    return Ok(first_term).release();
  }

  static Result<MathNode *> term(TokenIterator &cbegin,
                                 const TokenIterator &cend) {
    Result<unique_ptr<MathNode>> first_unary;
    extract_or_return(first_unary, unary(cbegin, cend));
    Token op;
    while ((op = match(cbegin, cend, {ASTERISK, SLASH})) != NONE) {
      Result<Unary *> second_unary = unary(cbegin, cend);
      return_if_error(second_unary);
      Ok(first_unary)
          .reset(new Term(Ok(first_unary).release(), op, Ok(second_unary)));
    }
    return Ok(first_unary).release();
  }

  static Result<Unary *> unary(TokenIterator &cbegin,
                               const TokenIterator &cend) {
    Token sign = match(cbegin, cend, {PLUS, MINUS});
    if (sign == NONE) {
      sign = Token("+", PLUS);
    }
    Result<Primary *> num = primary(cbegin, cend);
    return_if_error(num);
    return new Unary(sign, Ok(num));
  }

  static Result<Primary *> primary(TokenIterator &cbegin,
                                   const TokenIterator &cend) {
    if (cbegin->type == IDENT) {
      auto id = identifier(cbegin, cend);
      return_if_error(id);
      return Ok(id);
    }
    if (cbegin->type == NUMBER) {
      auto num = double_literal(cbegin, cend);
      return_if_error(num);
      return Ok(num);
    }
    return "Unexpected primary: \"" + cbegin->text + "\"";
  }

  static Result<ASTNode *> print_statement(TokenIterator &cbegin,
                                           const TokenIterator &cend) {
    ASTNode *to_be_printed;
    if (cbegin->type == STRING) {
      auto str = string_literal(cbegin, cend);
      return_if_error(str);
      to_be_printed = Ok(str);
    } else {
      auto expr = expression(cbegin, cend);
      return_if_error(expr);
      to_be_printed = Ok(expr);
    }
    return new Print(to_be_printed);
  }

  static Result<ASTNode *> if_statement(TokenIterator &cbegin,
                                        const TokenIterator &cend) {
    Result<unique_ptr<Comparison>> cmp;
    extract_or_return(cmp, comparison(cbegin, cend));
    if (match(cbegin, cend, {THEN}) == NONE) {
      return "Expected the keyword \"THEN\", but found \"" + cbegin->text +
             "\"";
    }
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    Result<unique_ptr<Block>> scope;
    extract_or_return(scope, block(cbegin, cend));
    if (match(cbegin, cend, {ENDIF}) == NONE) {
      return "Expected the keyword \"ENDIF\", but found \"" + cbegin->text +
             "\"";
    }
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new If(Ok(cmp).release(), Ok(scope).release());
  }

  static Result<ASTNode *> while_statement(TokenIterator &cbegin,
                                           const TokenIterator &cend) {
    Result<unique_ptr<Comparison>> cmp;
    extract_or_return(cmp, comparison(cbegin, cend));
    if (match(cbegin, cend, {REPEAT}) == NONE) {
      return "Expected the keyword \"REPEAT\", but found \"" + cbegin->text +
             "\"";
    }
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    Result<unique_ptr<Block>> scope;
    extract_or_return(scope, block(cbegin, cend));
    if (match(cbegin, cend, {ENDWHILE}) == NONE) {
      return "Expected the keyword \"ENDWHILE\", but found \"" + cbegin->text +
             "\"";
    }
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new While(Ok(cmp).release(), Ok(scope).release());
  }

  static Result<ASTNode *> label_statement(TokenIterator &cbegin,
                                           const TokenIterator &cend) {
    Result<unique_ptr<Identifier>> ident;
    extract_or_return(ident, identifier(cbegin, cend));
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new Label(Ok(ident).release());
  }

  static Result<ASTNode *> goto_statement(TokenIterator &cbegin,
                                          const TokenIterator &cend) {
    Result<unique_ptr<Identifier>> ident;
    extract_or_return(ident, identifier(cbegin, cend));
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new Goto(Ok(ident).release());
  }

  static Result<ASTNode *> let_statement(TokenIterator &cbegin,
                                         const TokenIterator &cend) {
    Result<unique_ptr<Identifier>> ident;
    extract_or_return(ident, identifier(cbegin, cend));
    if (match(cbegin, cend, {EQ}) == NONE) {
      return "A LET statement must contain '=' after the identifier, but "
             "found \"" +
             cbegin->text + "\"";
    }
    Result<unique_ptr<MathNode>> expr;
    extract_or_return(expr, expression(cbegin, cend));
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new Let(Ok(ident).release(), Ok(expr).release());
  }

  static Result<ASTNode *> input_statement(TokenIterator &cbegin,
                                           const TokenIterator &cend) {
    Result<unique_ptr<Identifier>> ident;
    extract_or_return(ident, identifier(cbegin, cend));
    if (!consume(cbegin, cend, NEWLINE)) {
      return "Expected EoL character, but found \"" + cbegin->text + "\"";
    }
    return new Input(get<1>(ident).release());
  }

  static Result<ASTNode *> statement(TokenIterator &cbegin,
                                     const TokenIterator &cend) {
    static unordered_map<
        TOKEN_TYPE,
        function<Result<ASTNode *>(TokenIterator &, const TokenIterator &)>>
        statement_functions{{PRINT, print_statement}, {IF, if_statement},
                            {WHILE, while_statement}, {LABEL, label_statement},
                            {GOTO, goto_statement},   {LET, let_statement},
                            {INPUT, input_statement}};
    consume(cbegin, cend, NEWLINE);
    if (cbegin == cend) {
      return nullptr;
    }
    auto statment_parser = statement_functions.find(cbegin->type);
    if (statment_parser == statement_functions.cend()) {
      return nullptr;
    }
    return statment_parser->second(++cbegin, cend);
  }

  static Result<Block *> block(TokenIterator &cbegin,
                               const TokenIterator &cend) {
    vector<unique_ptr<ASTNode>> statements;
    do {
      Result<unique_ptr<ASTNode>> stmt;
      extract_or_return(stmt, statement(cbegin, cend));
      statements.emplace_back(std::move(Ok(stmt)));
    } while (cbegin != cend && statements.back() != nullptr);
    if (statements.back() == nullptr) {
      statements.pop_back();
    }
    vector<ASTNode *> pure_statements;
    pure_statements.reserve(statements.size());
    for (auto &statement : statements) {
      pure_statements.push_back(statement.release());
    }
    return new Block(std::move(pure_statements));
  }
};

template <typename TokenIterator>
Result<unique_ptr<ASTNode>> parse(TokenIterator cbegin, TokenIterator cend) {
  Result<unique_ptr<ASTNode>> program_tree;
  extract_or_return(program_tree, Parser<TokenIterator>::block(cbegin, cend));
  if (Parser<TokenIterator>::match(cbegin, cend, {_EOF}) == NONE) {
    return "Error: Parser stopped before EoF";
  }
  return program_tree;
}

#undef Ok
#undef Error
#undef extract_or_return
#undef return_if_error
} // namespace Parser
