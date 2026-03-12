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
    // Grammar implementation and parsing logic omitted due to high-security proprietary details.
    // Core architectural patterns preserved in AST.h for demonstration.
};

} // namespace aero
