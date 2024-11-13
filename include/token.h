// Token.h
#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"

struct Token {
  TokenType type;
  std::string value;
  int line;
  int column;

  Token(TokenType type = TokenType::Unknown, const std::string& value = "",
        int line = 0, int column = 0)
      : type(type), value(value), line(line), column(column) {}
};

#endif  // TOKEN_H
