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
    std::unordered_map<std::string, Symbol> symbolTable;
    std::vector<std::string> errors;
    std::string currentFunctionReturnType;
    std::string lastExpressionType;
    
    void addError(const std::string& message);
    bool isDeclared(const std::string& name);
    Symbol* getSymbol(const std::string& name);
    void declareVariable(const std::string& name, const std::string& type, bool isMutable);
    bool typesCompatible(const std::string& type1, const std::string& type2);
};

} // namespace aero
