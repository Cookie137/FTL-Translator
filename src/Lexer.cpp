// Lexer.cpp
#include "../include/Lexer.h"

// Пропускает текущий символ и переходит к следующему
void Lexer::readChar() {
    if (readPosition >= input.size()) {
        currentChar = '\0'; // EOF
    } else {
        currentChar = input[readPosition];
    }
    position = readPosition;
    readPosition++;
    if (currentChar == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
}

// Возвращает следующий символ без его пропуска
char Lexer::peekChar() {
    if (readPosition >= input.size()) {
        return '\0';
    } else {
        return input[readPosition];
    }
}

// Пропускает пробельные символы (пробелы, табуляции, переводы строк)
void Lexer::skipWhitespace() {
    while (isspace(currentChar)) {
        readChar();
    }
}

// Пропускает комментарии (однострочные // и многострочные /* */)
void Lexer::skipComment() {
    if (currentChar == '/') {
        char next = peekChar();
        if (next == '/') {
            // Однострочный комментарий
            while (currentChar != '\n' && currentChar != '\0') {
                readChar();
            }
        } else if (next == '*') {
            // Многострочный комментарий
            readChar(); // Пропустить '/'
            readChar(); // Пропустить '*'
            while (true) {
                if (currentChar == '*' && peekChar() == '/') {
                    readChar(); // Пропустить '*'
                    readChar(); // Пропустить '/'
                    break;
                } else if (currentChar == '\0') {
                    // Ошибка: не закрыт многострочный комментарий
                    break;
                } else {
                    readChar();
                }
            }
        }
    }
}

// Основной метод для токенизации входного кода
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token tok;

    while (true) {
        skipWhitespace();

        // Проверка на комментарии
        if (currentChar == '/') {
            char next = peekChar();
            if (next == '/' || next == '*') {
                skipComment();
                skipWhitespace();
                if (currentChar == '\0') {
                    break;
                }
            }
        }

        tok = nextToken();
        tokens.push_back(tok);
        if (tok.type == TokenType::EndOfFile) {
            break;
        }
    }

    return tokens;
}

// Генерация следующего токена
Token Lexer::nextToken() {
    skipWhitespace();

    Token tok;
    tok.line = line;
    tok.column = column;

    switch (currentChar) {
        // Разделители и операторы
        case '=':
            if (peekChar() == '=') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_EQUAL;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::OP_ASSIGN;
                tok.value = "=";
            }
            break;
        case '+':
            tok.type = TokenType::OP_PLUS;
            tok.value = "+";
            break;
        case '-':
            if (peekChar() == '>') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_ARROW;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::OP_MINUS;
                tok.value = "-";
            }
            break;
        case '*':
            tok.type = TokenType::OP_MULTIPLY;
            tok.value = "*";
            break;
        case '/':
            tok.type = TokenType::OP_DIVIDE;
            tok.value = "/";
            break;
        case '%':
            tok.type = TokenType::OP_MODULO;
            tok.value = "%";
            break;
        case '!':
            if (peekChar() == '=') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_NOT_EQUAL;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::OP_NOT;
                tok.value = "!";
            }
            break;
        case '<':
            if (peekChar() == '=') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_LESS_EQUAL;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::OP_LESS;
                tok.value = "<";
            }
            break;
        case '>':
            if (peekChar() == '=') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_GREATER_EQUAL;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::OP_GREATER;
                tok.value = ">";
            }
            break;
        case '&':
            if (peekChar() == '&') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_AND;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::Unknown;
                tok.value = "&";
            }
            break;
        case '|':
            if (peekChar() == '|') {
                char prev = currentChar;
                readChar();
                tok.type = TokenType::OP_OR;
                tok.value = std::string(1, prev) + std::string(1, currentChar);
            } else {
                tok.type = TokenType::Unknown;
                tok.value = "|";
            }
            break;
        case '(':
            tok.type = TokenType::LPAREN;
            tok.value = "(";
            break;
        case ')':
            tok.type = TokenType::RPAREN;
            tok.value = ")";
            break;
        case '{':
            tok.type = TokenType::LBRACE;
            tok.value = "{";
            break;
        case '}':
            tok.type = TokenType::RBRACE;
            tok.value = "}";
            break;
        case '[':
            tok.type = TokenType::LBRACKET;
            tok.value = "[";
            break;
        case ']':
            tok.type = TokenType::RBRACKET;
            tok.value = "]";
            break;
        case ',':
            tok.type = TokenType::COMMA;
            tok.value = ",";
            break;
        case ':':
            tok.type = TokenType::COLON;
            tok.value = ":";
            break;
        case ';':
            tok.type = TokenType::SEMICOLON;
            tok.value = ";";
            break;
        case '.':
            tok.type = TokenType::DOT;
            tok.value = ".";
            break;
        case '\0':
            tok.type = TokenType::EndOfFile;
            tok.value = "";
            break;
        case '"':
            tok = stringLiteral();
            return tok;
        default:
            if (isalpha(currentChar) || currentChar == '_') {
                tok = identifier();
                return tok;
            } else if (isdigit(currentChar)) {
                tok = number();
                return tok;
            } else {
                tok.type = TokenType::Unknown;
                tok.value = std::string(1, currentChar);
            }
    }

    readChar();
    return tok;
}

// Обработка идентификаторов и ключевых слов
Token Lexer::identifier() {
    size_t startPos = position;
    int startColumn = column;
    while (isalnum(currentChar) || currentChar == '_') {
        readChar();
    }
    std::string ident = input.substr(startPos, readPosition - position - 1);
    TokenType type = checkKeyword(ident);
    if (type == TokenType::KW_TRUE || type == TokenType::KW_FALSE) {
        return Token(TokenType::BooleanLiteral, ident, line, startColumn);
    }
    return Token(type, ident, line, startColumn);
}

// Обработка числовых литералов (целые числа)
Token Lexer::number() {
    size_t startPos = position;
    int startColumn = column;
    while (isdigit(currentChar)) {
        readChar();
    }
    std::string numStr = input.substr(startPos, readPosition - position - 1);
    return Token(TokenType::IntegerLiteral, numStr, line, startColumn);
}

// Обработка строковых литералов
Token Lexer::stringLiteral() {
    int startColumn = column;
    readChar(); // Пропустить начальную кавычку
    size_t startPos = position;
    std::string str;
    while (currentChar != '"' && currentChar != '\0') {
        if (currentChar == '\\') {
            readChar();
            switch (currentChar) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case '"': str += '"'; break;
                case '\\': str += '\\'; break;
                // Добавьте другие управляющие последовательности по необходимости
                default: str += currentChar; break;
            }
        } else {
            str += currentChar;
        }
        readChar();
    }
    readChar(); // Пропустить закрывающую кавычку
    return Token(TokenType::StringLiteral, str, line, startColumn);
}

// Обработка списковых литералов [1, 2, 3]
Token Lexer::listLiteral() {
    int startColumn = column;
    std::string listStr;
    listStr += currentChar; // Добавить '['
    readChar(); // Пропустить '['

    while (currentChar != ']' && currentChar != '\0') {
        if (currentChar == '"') {
            Token strTok = stringLiteral();
            listStr += "\"" + strTok.value + "\"";
            continue;
        }
        listStr += currentChar;
        readChar();
    }

    if (currentChar == ']') {
        listStr += currentChar;
        readChar(); // Пропустить ']'
    }

    return Token(TokenType::ListLiteral, listStr, line, startColumn);
}

// Обработка объектных литералов { "key": "value" }
Token Lexer::objectLiteral() {
    int startColumn = column;
    std::string objStr;
    objStr += currentChar; // Добавить '{'
    readChar(); // Пропустить '{'

    while (currentChar != '}' && currentChar != '\0') {
        if (currentChar == '"') {
            Token strTok = stringLiteral();
            objStr += "\"" + strTok.value + "\"";
            continue;
        }
        objStr += currentChar;
        readChar();
    }

    if (currentChar == '}') {
        objStr += currentChar;
        readChar(); // Пропустить '}'
    }

    return Token(TokenType::ObjectLiteral, objStr, line, startColumn);
}
