#include "../include/Lexer.h"
#include <iostream>
#include <cassert>

void testBasicTokens() {
    std::string source = "activity MainActivity { }";
    aero::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == aero::TokenType::ACTIVITY);
    assert(tokens[1].type == aero::TokenType::IDENTIFIER);
    assert(tokens[2].type == aero::TokenType::LBRACE);
    assert(tokens[3].type == aero::TokenType::RBRACE);
    assert(tokens[4].type == aero::TokenType::END_OF_FILE);
    
    std::cout << "✓ Basic tokens test passed\n";
}

void testStringLiteral() {
    std::string source = R"("Hello, World!")";
    aero::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == aero::TokenType::STRING);
    assert(tokens[0].lexeme == "Hello, World!");
    
    std::cout << "✓ String literal test passed\n";
}

void testNumbers() {
    std::string source = "42 3.14";
    aero::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(tokens[0].type == aero::TokenType::NUMBER);
    assert(tokens[0].lexeme == "42");
    assert(tokens[1].type == aero::TokenType::NUMBER);
    assert(tokens[1].lexeme == "3.14");
    
    std::cout << "✓ Number test passed\n";
}

int main() {
    std::cout << "Running Lexer tests...\n";
    
    testBasicTokens();
    testStringLiteral();
    testNumbers();
    
    std::cout << "\nAll tests passed! ✓\n";
    return 0;
}
