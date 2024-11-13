// main.cpp
#include <iostream>

#include "../include/Lexer.h"

// Функция для получения строкового представления типа токена
std::string tokenTypeToString(TokenType type) {
  switch (type) {
    case TokenType::EndOfFile:
      return "EndOfFile";
    case TokenType::Unknown:
      return "Unknown";
    case TokenType::Identifier:
      return "Identifier";
    case TokenType::IntegerLiteral:
      return "IntegerLiteral";
    case TokenType::StringLiteral:
      return "StringLiteral";
    case TokenType::BooleanLiteral:
      return "BooleanLiteral";
    case TokenType::ListLiteral:
      return "ListLiteral";
    case TokenType::ObjectLiteral:
      return "ObjectLiteral";
    case TokenType::KW_FUNCTION:
      return "KW_FUNCTION";
    case TokenType::KW_VAR:
      return "KW_VAR";
    case TokenType::KW_LET:
      return "KW_LET";
    case TokenType::KW_STRUCT:
      return "KW_STRUCT";
    case TokenType::KW_EXCEPTION:
      return "KW_EXCEPTION";
    case TokenType::KW_MIDDLEWARE:
      return "KW_MIDDLEWARE";
    case TokenType::KW_ENDPOINT:
      return "KW_ENDPOINT";
    case TokenType::KW_APPLY:
      return "KW_APPLY";
    case TokenType::KW_IF:
      return "KW_IF";
    case TokenType::KW_ELSE:
      return "KW_ELSE";
    case TokenType::KW_RETURN:
      return "KW_RETURN";
    case TokenType::KW_MATCH:
      return "KW_MATCH";
    case TokenType::KW_THROW:
      return "KW_THROW";
    case TokenType::KW_FOR:
      return "KW_FOR";
    case TokenType::KW_IN:
      return "KW_IN";
    case TokenType::KW_WHILE:
      return "KW_WHILE";
    case TokenType::KW_ASYNC:
      return "KW_ASYNC";
    case TokenType::KW_AWAIT:
      return "KW_AWAIT";
    case TokenType::KW_TRUE:
      return "KW_TRUE";
    case TokenType::KW_FALSE:
      return "KW_FALSE";
    case TokenType::OP_ASSIGN:
      return "OP_ASSIGN";
    case TokenType::OP_PLUS:
      return "OP_PLUS";
    case TokenType::OP_MINUS:
      return "OP_MINUS";
    case TokenType::OP_MULTIPLY:
      return "OP_MULTIPLY";
    case TokenType::OP_DIVIDE:
      return "OP_DIVIDE";
    case TokenType::OP_MODULO:
      return "OP_MODULO";
    case TokenType::OP_EQUAL:
      return "OP_EQUAL";
    case TokenType::OP_NOT_EQUAL:
      return "OP_NOT_EQUAL";
    case TokenType::OP_LESS:
      return "OP_LESS";
    case TokenType::OP_GREATER:
      return "OP_GREATER";
    case TokenType::OP_LESS_EQUAL:
      return "OP_LESS_EQUAL";
    case TokenType::OP_GREATER_EQUAL:
      return "OP_GREATER_EQUAL";
    case TokenType::OP_AND:
      return "OP_AND";
    case TokenType::OP_OR:
      return "OP_OR";
    case TokenType::OP_NOT:
      return "OP_NOT";
    case TokenType::OP_ARROW:
      return "OP_ARROW";
    case TokenType::OP_DOUBLE_ARROW:
      return "OP_DOUBLE_ARROW";
    case TokenType::LPAREN:
      return "LPAREN";
    case TokenType::RPAREN:
      return "RPAREN";
    case TokenType::LBRACE:
      return "LBRACE";
    case TokenType::RBRACE:
      return "RBRACE";
    case TokenType::LBRACKET:
      return "LBRACKET";
    case TokenType::RBRACKET:
      return "RBRACKET";
    case TokenType::COMMA:
      return "COMMA";
    case TokenType::COLON:
      return "COLON";
    case TokenType::SEMICOLON:
      return "SEMICOLON";
    case TokenType::DOT:
      return "DOT";
    default:
      return "Other";
  }
}

int main() {
  std::string code = R"(
        // Объявление функции суммы
        function sum(a: Int, b: Int): Int {
            return a + b;
        }

        /* 
           Асинхронная функция для получения данных
        */
        async function fetchData(url: String): Response {
            let response = await httpGet(url);
            return response;
        }

        // Пример использования цикла for
        function printUsers(users: List<User>) {
            for (user in users) {
                println("User: " + user.name + ", ID: " + user.id);
            }
        }

        // Пример использования цикла while
        function countdown(start: Int) {
            let current = start;
            while (current > 0) {
                println("Countdown: " + current);
                current = current - 1;
            }
            println("Liftoff!");
        }

        // Пример работы с объектным литералом
        let user = {
            id: 1,
            name: "Alice",
            email: "alice@example.com"
        };

        // Пример работы со списковым литералом
        let numbers = [1, 2, 3, 4, 5];

        // Пример работы с булевыми значениями
        let isActive = true;
        let isVerified = false;

        // Пример использования throw
        function createUser(name: String) {
            if (name == "") {
                throw BadRequestError { message: "Name cannot be empty" };
            }
            // Создание пользователя
        }
    )";

  Lexer lexer(code);
  std::vector<Token> tokens = lexer.tokenize();

  for (const auto& token : tokens) {
    std::cout << "Token(Type: " << tokenTypeToString(token.type)
              << ", Value: \"" << token.value << "\""
              << ", Line: " << token.line << ", Column: " << token.column
              << ")\n";
  }

  return 0;
}
