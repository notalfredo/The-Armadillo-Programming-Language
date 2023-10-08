#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class CodeGenContext;
class NodeStatement;
class NodeExpression;
class NodeVariableDeclaration;

typedef std::vector<NodeStatement*> StatementList;
typedef std::vector<NodeExpression*> ExpressionList;
typedef std::vector<NodeVariableDeclaration*> VariableList;

class Node {
public:
	virtual ~Node() {}
	virtual llvm::Value* codeGen(CodeGenContext& context) { 
        return NULL; 
    }
};

class NodeStatement: public Node {};

class NodeExpression : public Node {};

class NodeInteger : public NodeExpression {
public:
    long long value;
    NodeInteger( long long value ) : value(value) {};
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeDouble : public NodeExpression {
public:
    double value;
    NodeDouble( double value ) : value(value) {};
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeIdentifier : public NodeExpression {
public:
    std::string name; 
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeMethodCall : public NodeExpression {
public:
    const NodeIdentifier& id;
    ExpressionList arguments;

    NodeMethodCall(
        const NodeIdentifier& id,
        ExpressionList& arguments
    ): id(id), arguments(arguments) {}

    
    NodeMethodCall(
        const NodeIdentifier& id
    ): id(id) {}


    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeBinaryOperator : public NodeExpression {
public:
    NodeExpression &lhs;   
    NodeExpression &rhs;   
    int BinaryOperatorType;
    NodeBinaryOperator( 
                        NodeExpression& lhs, 
                        NodeExpression& rhs, 
                        int BinaryOperatorType
    ): lhs(lhs), rhs(rhs), BinaryOperatorType(BinaryOperatorType) {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeAssignment : public NodeExpression {
public:
    NodeIdentifier& lhs;
    NodeExpression& rhs;
    
    NodeAssignment(
            NodeIdentifier& lhs,
            NodeExpression& rhs
    ): lhs(lhs), rhs(rhs) {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeBlock : public NodeExpression {
public:
    StatementList StatementList;
    NodeBlock() {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeExpressionStatement : public NodeStatement {
public:
    NodeExpression& expr;
    NodeExpressionStatement(NodeExpression& expr): expr(expr) {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeReturnStatement : public NodeStatement {
public:
    NodeExpression& expr;
    NodeReturnStatement(NodeExpression& expr): expr(expr) {}
     
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeVariableDeclaration : public NodeStatement {
public:
    const std::string type;
    NodeIdentifier& id;

    NodeExpression* assignExpr;

    NodeVariableDeclaration(
        const std::string type,
        NodeIdentifier& id
        ): type(type), id(id) { assignExpr = NULL; }
    
    NodeVariableDeclaration(
        const std::string type,
        NodeIdentifier& id,
        NodeExpression* assignExpr
        ): type(type), id(id), assignExpr(assignExpr){}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NodeFunctionDeclaration : public NodeStatement {
public:

    NodeIdentifier& id;
    const std::string returnType;
    VariableList params;
    NodeBlock& body;

    NodeFunctionDeclaration(
        NodeIdentifier& id,
        const std::string returnType,
        VariableList params,
        NodeBlock& body
    ): id(id), returnType(returnType), params(params), body(body) {} 

    virtual llvm::Value* codeGen(CodeGenContext& context);
};


class NodeIfDeclaration:  public NodeStatement {
public:
    ExpressionList exprs;
    NodeBlock& body;
    NodeStatement& elif;

    NodeIfDeclaration(
        ExpressionList& exprs,
        NodeBlock& body,
        NodeStatement& elif
    ): exprs(exprs), body(body), elif(elif) {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};


class NodeElifDeclaration:  public NodeStatement {
public:
    ExpressionList exprs;
    NodeBlock& body;


    NodeElifDeclaration(
        ExpressionList& exprs,
        NodeBlock& body
    ): exprs(exprs), body(body) {}

    virtual llvm::Value* codeGen(CodeGenContext& context);
};
