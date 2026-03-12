#pragma once

#include "Lexer.h"
#include "AST.h"
#include <memory>
#include <stdexcept>

namespace aero {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    std::unique_ptr<Program> parse();
    
private:
    std::vector<Token> tokens;
    size_t current;
    
    // Helpers
    Token peek() const;
    Token previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    
    void error(const std::string& message);
    
    // Parsing methods
    std::unique_ptr<Statement> declaration();
    std::unique_ptr<Statement> activityDeclaration();
    std::unique_ptr<Statement> fragmentDeclaration();
    std::unique_ptr<Statement> serviceDeclaration();
    std::unique_ptr<Statement> classDeclaration();
    std::unique_ptr<Statement> functionDeclaration();
    std::unique_ptr<Statement> variableDeclaration();
    std::string parseTypeAnnotation();  // parses Identifier or Identifier<T> or Identifier<K,V>
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> ifStatement();
    std::unique_ptr<Statement> whileStatement();
    std::unique_ptr<Statement> forStatement();
    std::unique_ptr<Statement> whenStatement();
    std::unique_ptr<Statement> returnStatement();
    std::unique_ptr<Statement> expressionStatement();
    std::vector<std::unique_ptr<Statement>> block();
    
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> primary();
};

} // namespace aero
