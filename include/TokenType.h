// TokenType.h
#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <string>
#include <unordered_set>

enum class TokenType {
  // Специальные токены
  EndOfFile,
  Unknown,

  // Идентификаторы и литералы
  Identifier,
  IntegerLiteral,
  StringLiteral,
  BooleanLiteral,
  ListLiteral,
  ObjectLiteral,

  // Ключевые слова
  KW_FUNCTION,
  KW_VAR,
  KW_LET,
  KW_STRUCT,
  KW_EXCEPTION,
  KW_MIDDLEWARE,
  KW_ENDPOINT,
  KW_APPLY,
  KW_IF,
  KW_ELSE,
  KW_RETURN,
  KW_MATCH,
  KW_THROW,
  KW_FOR,
  KW_IN,
  KW_WHILE,
  KW_ASYNC,
  KW_AWAIT,
  KW_TRUE,
  KW_FALSE,
  KW_UNDEFINED,
  KW_NULL,

  // Операторы
  OP_ASSIGN,         // =
  OP_PLUS,           // +
  OP_MINUS,          // -
  OP_MULTIPLY,       // *
  OP_DIVIDE,         // /
  OP_MODULO,         // %
  OP_EQUAL,          // ==
  OP_NOT_EQUAL,      // !=
  OP_LESS,           // <
  OP_GREATER,        // >
  OP_LESS_EQUAL,     // <=
  OP_GREATER_EQUAL,  // >=
  OP_AND,            // &&
  OP_OR,             // ||
  OP_NOT,            // !
  OP_ARROW,          // =>
  OP_DOUBLE_ARROW,  // => (для сопоставления с образцом)

  // Разделители
  LPAREN,     // (
  RPAREN,     // )
  LBRACE,     // {
  RBRACE,     // }
  LBRACKET,   // [
  RBRACKET,   // ]
  COMMA,      // ,
  COLON,      // :
  SEMICOLON,  // ;
  DOT,        // .

  // Другие
  // Добавьте другие токены по необходимости
};

// Функция для проверки, является ли строка ключевым словом
inline TokenType checkKeyword(const std::string& str) {
  static const std::unordered_set<std::string> keywords = {
      "function",   "var",      "let",   "struct", "exception",
      "middleware", "endpoint", "apply", "if",     "else",
      "return",     "match",    "throw", "for",    "in",
      "while",      "async",    "await", "true",   "false",
      "undefined", "null",
    };

  if (keywords.find(str) != keywords.end()) {
    if (str == "function") return TokenType::KW_FUNCTION;
    if (str == "var") return TokenType::KW_VAR;
    if (str == "let") return TokenType::KW_LET;
    if (str == "struct") return TokenType::KW_STRUCT;
    if (str == "exception") return TokenType::KW_EXCEPTION;
    if (str == "middleware") return TokenType::KW_MIDDLEWARE;
    if (str == "endpoint") return TokenType::KW_ENDPOINT;
    if (str == "apply") return TokenType::KW_APPLY;
    if (str == "if") return TokenType::KW_IF;
    if (str == "else") return TokenType::KW_ELSE;
    if (str == "return") return TokenType::KW_RETURN;
    if (str == "match") return TokenType::KW_MATCH;
    if (str == "throw") return TokenType::KW_THROW;
    if (str == "for") return TokenType::KW_FOR;
    if (str == "in") return TokenType::KW_IN;
    if (str == "while") return TokenType::KW_WHILE;
    if (str == "async") return TokenType::KW_ASYNC;
    if (str == "await") return TokenType::KW_AWAIT;
    if (str == "true") return TokenType::KW_TRUE;
    if (str == "false") return TokenType::KW_FALSE;
    if (str == "undefined") return TokenType::KW_UNDEFINED;
    if (str == "null") return TokenType::KW_NULL;
  }
  return TokenType::Identifier;
}

#endif  // TOKENTYPE_H
