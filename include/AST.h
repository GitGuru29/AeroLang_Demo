#pragma once

#include <memory>
#include <string>
#include <vector>

namespace aero {

// Forward declarations
class ASTVisitor;

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// Expressions
class Expression : public ASTNode {};

class NumberLiteral : public Expression {
public:
    double value;
    explicit NumberLiteral(double value) : value(value) {}
    void accept(ASTVisitor& visitor) override;
};

class StringLiteral : public Expression {
public:
    std::string value;
    explicit StringLiteral(const std::string& value) : value(value) {}
    void accept(ASTVisitor& visitor) override;
};

class BooleanLiteral : public Expression {
public:
    bool value;
    explicit BooleanLiteral(bool value) : value(value) {}
    void accept(ASTVisitor& visitor) override;
};

class Identifier : public Expression {
public:
    std::string name;
    explicit Identifier(const std::string& name) : name(name) {}
    void accept(ASTVisitor& visitor) override;
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;
    
    BinaryExpression(std::unique_ptr<Expression> left, const std::string& op, 
                     std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class CallExpression : public Expression {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    CallExpression(const std::string& callee, 
                   std::vector<std::unique_ptr<Expression>> arguments)
        : callee(callee), arguments(std::move(arguments)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class MemberAccessExpression : public Expression {
public:
    std::unique_ptr<Expression> object;
    std::string member;
    
    MemberAccessExpression(std::unique_ptr<Expression> object, const std::string& member)
        : object(std::move(object)), member(member) {}
    
    void accept(ASTVisitor& visitor) override;
};

class MemberCallExpression : public Expression {
public:
    std::unique_ptr<Expression> object;
    std::string method;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    MemberCallExpression(std::unique_ptr<Expression> object, const std::string& method,
                         std::vector<std::unique_ptr<Expression>> arguments)
        : object(std::move(object)), method(method), arguments(std::move(arguments)) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Statements
class Statement : public ASTNode {};

class LambdaExpression : public Expression {
public:
    std::vector<std::pair<std::string, std::string>> parameters;
    std::vector<std::unique_ptr<Statement>> body;
    
    LambdaExpression(std::vector<std::pair<std::string, std::string>> parameters,
                     std::vector<std::unique_ptr<Statement>> body)
        : parameters(std::move(parameters)), body(std::move(body)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class VariableDeclaration : public Statement {
public:
    bool isMutable; // var vs val
    std::string name;
    std::string type;
    std::unique_ptr<Expression> initializer;
    int sourceLine = 0; // .aero line number for source maps
    
    VariableDeclaration(bool isMutable, const std::string& name, 
                        const std::string& type, 
                        std::unique_ptr<Expression> initializer,
                        int sourceLine = 0)
        : isMutable(isMutable), name(name), type(type), 
          initializer(std::move(initializer)), sourceLine(sourceLine) {}
    
    void accept(ASTVisitor& visitor) override;
};

class FunctionDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::pair<std::string, std::string>> parameters; // (name, type)
    std::string returnType;
    std::vector<std::unique_ptr<Statement>> body;
    
    FunctionDeclaration(const std::string& name,
                        std::vector<std::pair<std::string, std::string>> parameters,
                        const std::string& returnType,
                        std::vector<std::unique_ptr<Statement>> body)
        : name(name), parameters(std::move(parameters)), 
          returnType(returnType), body(std::move(body)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ClassDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::unique_ptr<VariableDeclaration>> fields;
    std::vector<std::unique_ptr<FunctionDeclaration>> methods;
    
    ClassDeclaration(const std::string& name,
                     std::vector<std::unique_ptr<VariableDeclaration>> fields,
                     std::vector<std::unique_ptr<FunctionDeclaration>> methods)
        : name(name), fields(std::move(fields)), methods(std::move(methods)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ActivityDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::unique_ptr<VariableDeclaration>> fields;
    std::vector<std::unique_ptr<FunctionDeclaration>> methods;
    
    ActivityDeclaration(const std::string& name,
                        std::vector<std::unique_ptr<VariableDeclaration>> fields,
                        std::vector<std::unique_ptr<FunctionDeclaration>> methods)
        : name(name), fields(std::move(fields)), methods(std::move(methods)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class FragmentDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::unique_ptr<VariableDeclaration>> fields;
    std::vector<std::unique_ptr<FunctionDeclaration>> methods;
    
    FragmentDeclaration(const std::string& name,
                        std::vector<std::unique_ptr<VariableDeclaration>> fields,
                        std::vector<std::unique_ptr<FunctionDeclaration>> methods)
        : name(name), fields(std::move(fields)), methods(std::move(methods)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ServiceDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::unique_ptr<VariableDeclaration>> fields;
    std::vector<std::unique_ptr<FunctionDeclaration>> methods;
    
    ServiceDeclaration(const std::string& name,
                       std::vector<std::unique_ptr<VariableDeclaration>> fields,
                       std::vector<std::unique_ptr<FunctionDeclaration>> methods)
        : name(name), fields(std::move(fields)), methods(std::move(methods)) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Control Flow Statements

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> thenBody;
    std::vector<std::unique_ptr<Statement>> elseBody; // empty if no else
    
    IfStatement(std::unique_ptr<Expression> condition,
                std::vector<std::unique_ptr<Statement>> thenBody,
                std::vector<std::unique_ptr<Statement>> elseBody)
        : condition(std::move(condition)), thenBody(std::move(thenBody)),
          elseBody(std::move(elseBody)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> body;
    
    WhileStatement(std::unique_ptr<Expression> condition,
                   std::vector<std::unique_ptr<Statement>> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ForStatement : public Statement {
public:
    std::string varName;
    std::unique_ptr<Expression> rangeStart;
    std::unique_ptr<Expression> rangeEnd;
    std::vector<std::unique_ptr<Statement>> body;
    
    ForStatement(const std::string& varName,
                 std::unique_ptr<Expression> rangeStart,
                 std::unique_ptr<Expression> rangeEnd,
                 std::vector<std::unique_ptr<Statement>> body)
        : varName(varName), rangeStart(std::move(rangeStart)),
          rangeEnd(std::move(rangeEnd)), body(std::move(body)) {}
    
    void accept(ASTVisitor& visitor) override;
};

struct WhenCase {
    std::unique_ptr<Expression> value; // nullptr = else case
    std::vector<std::unique_ptr<Statement>> body;
};

class WhenStatement : public Statement {
public:
    std::unique_ptr<Expression> subject;
    std::vector<WhenCase> cases;
    
    WhenStatement(std::unique_ptr<Expression> subject,
                  std::vector<WhenCase> cases)
        : subject(std::move(subject)), cases(std::move(cases)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class BreakStatement : public Statement {
public:
    void accept(ASTVisitor& visitor) override;
};

class ContinueStatement : public Statement {
public:
    void accept(ASTVisitor& visitor) override;
};

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value;
    int sourceLine = 0;
    
    ReturnStatement(std::unique_ptr<Expression> value, int sourceLine = 0)
        : value(std::move(value)), sourceLine(sourceLine) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    int sourceLine = 0;
    
    ExpressionStatement(std::unique_ptr<Expression> expression, int sourceLine = 0)
        : expression(std::move(expression)), sourceLine(sourceLine) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Program (root node)
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    explicit Program(std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements)) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Visitor pattern for traversing AST
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(NumberLiteral&) = 0;
    virtual void visit(StringLiteral&) = 0;
    virtual void visit(BooleanLiteral&) = 0;
    virtual void visit(Identifier&) = 0;
    virtual void visit(BinaryExpression&) = 0;
    virtual void visit(CallExpression&) = 0;
    virtual void visit(MemberAccessExpression&) = 0;
    virtual void visit(MemberCallExpression&) = 0;
    virtual void visit(LambdaExpression&) = 0;
    virtual void visit(VariableDeclaration&) = 0;
    virtual void visit(FunctionDeclaration&) = 0;
    virtual void visit(ClassDeclaration&) = 0;
    virtual void visit(ActivityDeclaration&) = 0;
    virtual void visit(FragmentDeclaration&) = 0;
    virtual void visit(ServiceDeclaration&) = 0;
    virtual void visit(IfStatement&) = 0;
    virtual void visit(WhileStatement&) = 0;
    virtual void visit(ForStatement&) = 0;
    virtual void visit(WhenStatement&) = 0;
    virtual void visit(BreakStatement&) = 0;
    virtual void visit(ContinueStatement&) = 0;
    virtual void visit(ReturnStatement&) = 0;
    virtual void visit(ExpressionStatement&) = 0;
    virtual void visit(Program&) = 0;
};

} // namespace aero
