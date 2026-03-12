#pragma once

#include <string>
#include <vector>

namespace aero {

enum class TokenType {
    // Literals
    IDENTIFIER,
    NUMBER,
    STRING,
    
    // Keywords
    ACTIVITY,
    FRAGMENT,
    CLASS,
    FUN,
    VAR,
    VAL,
    IF,
    ELSE,
    WHEN,
    FOR,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    NULL_LITERAL,
    OVERRIDE,
    PRIVATE,
    PUBLIC,
    PROTECTED,
    INTENT,
    SERVICE,
    THIS,
    IN,
    BREAK,
    CONTINUE,
    
    // Operators
    PLUS,           // +
    MINUS,          // -
    STAR,           // *
    SLASH,          // /
    PERCENT,        // %
    EQUALS,         // =
    EQUALS_EQUALS,  // ==
    NOT_EQUALS,     // !=
    LESS,           // <
    LESS_EQUALS,    // <=
    GREATER,        // >
    GREATER_EQUALS, // >=
    AND,            // &&
    OR,             // ||
    NOT,            // !
    
    // Delimiters
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    COMMA,          // ,
    DOT,            // .
    DOT_DOT,        // ..
    COLON,          // :
    SCOPE,          // ::
    SEMICOLON,      // ;
    ARROW,          // ->
    FAT_ARROW,      // =>
    
    // Special
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column) {}
};

class Lexer {
public:
    Lexer(const std::string& source);
    
    std::vector<Token> tokenize();
    
private:
    std::string source;
    size_t current;
    int line;
    int column;
    
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    bool match(char expected);
    
    void skipWhitespace();
    void skipComment();
    
    Token makeToken(TokenType type, const std::string& lexeme);
    Token scanToken();
    Token scanNumber();
    Token scanString();
    Token scanIdentifier();
    
    TokenType identifierType(const std::string& text);
};

} // namespace aero
