#pragma once

#include "AST.h"
#include <sstream>
#include <string>
#include <vector>

namespace aero {

// One entry in the source map: maps a .aero source line to a generated C++ line
struct SourceMapEntry {
    int aeroLine;  // 1-based line in the .aero source file
    int cppLine;   // 1-based line in the generated .cpp file
};

class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();
    
    std::string generate(Program& program);
    std::string getGeneratedCode() const;

    // Source map access (call after generate())
    const std::vector<SourceMapEntry>& getSourceMap() const { return sourceMapEntries; }
    void resetSourceMap() { sourceMapEntries.clear(); outputLine = 1; }
    
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
    std::stringstream output;
    int indentLevel;
    int outputLine;                           // tracks current line in the output .cpp
    std::vector<SourceMapEntry> sourceMapEntries; // accumulated source map
    
    void indent();
    void emit(const std::string& code);
    void emitLine(const std::string& code);
    void emitStmt(const std::string& code, int aeroLine); // emit + record source map entry
    std::string mapType(const std::string& aeroType);
    std::string mapOperator(const std::string& op);
};

} // namespace aero
