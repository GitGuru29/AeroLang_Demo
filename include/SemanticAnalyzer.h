#pragma once

#include "AST.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace aero {

// Symbol table entry
struct Symbol {
    std::string name;
    std::string type;
    bool isMutable;
    bool isInitialized;
};

// Semantic analyzer for type checking and validation
class SemanticAnalyzer : public ASTVisitor {
public:
    SemanticAnalyzer();
    
    bool analyze(Program& program);
    std::vector<std::string> getErrors() const;
    
    // Visitor implementations
    void visit(NumberLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(MemberAccessExpression& node) override;
    void visit(MemberCallExpression& node) override;
    void visit(LambdaExpression& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(FunctionDeclaration& node) override;
    void visit(ClassDeclaration& node) override;
    void visit(ActivityDeclaration& node) override;
    void visit(FragmentDeclaration& node) override;
    void visit(ServiceDeclaration& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(WhenStatement& node) override;
    void visit(BreakStatement& node) override;
    void visit(ContinueStatement& node) override;
    void visit(ReturnStatement& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(Program& node) override;
    
private:
    // Type system implementation and symbol table logic omitted due to high-security proprietary details.
    // Public validation errors and AST traversal logic preserved for demonstration.
};

} // namespace aero
