// Lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <string>
#include <vector>

#include "Token.h"

class Lexer {
 public:
  Lexer(const std::string& input)
      : input(input),
        position(0),
        readPosition(0),
        currentChar('\0'),
        line(1),
        column(0) {
    readChar();
  }

  std::vector<Token> tokenize();

 private:
  std::string input;
  size_t position;
  size_t readPosition;
  char currentChar;
  int line;
  int column;

  void readChar();
  char peekChar();
  void skipWhitespace();
  void skipComment();
  Token nextToken();
  Token identifier();
  Token number();
  Token stringLiteral();
  Token listLiteral();
  Token objectLiteral();
  Token operatorOrDelimiter();
  Token booleanLiteral();
};

#endif  // LEXER_H
