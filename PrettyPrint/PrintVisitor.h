#ifndef PRINTVISITOR_H
#define PRINTVISITOR_H
#include "common.h"

#include <fstream>
#include <vector>

#include "TreeTypes.h"

namespace AST {

class PrintVisitor : public IVisitor {
public:
    PrintVisitor(std::string filename) :
        dot(filename)
    {}

    ~PrintVisitor() {
        dot.close();
    }

    void CreateGraph(Program* program);

protected:
    virtual void Visit(const Program *node) override;
    virtual void Visit(const MainClass *node) override;
    virtual void Visit(const ClassDeclaration *node) override;
    virtual void Visit(const VarDeclaration *node) override;
    virtual void Visit(const MethodDeclaration *node) override;
    virtual void Visit(const Sequence<const ClassDeclaration> *node) override;
    virtual void Visit(const Sequence<const VarDeclaration> *node) override;
    virtual void Visit(const Sequence<const MethodDeclaration> *node) override;
    virtual void Visit(const Qualifier *node) override;
    virtual void Visit(const Type *node) override;
    virtual void Visit(const ReturnStatement *node) override;
    virtual void Visit(const AssignArrayElementStatement *node) override;
    virtual void Visit(const AssignStatement *node) override;
    virtual void Visit(const PrintLineStatement *node) override;
    virtual void Visit(const WhileStatement *node) override;
    virtual void Visit(const BraceSequenceStatement *node) override;
    virtual void Visit(const IfElseStatement *node) override;
    virtual void Visit(const Sequence<const IStatement> *node) override;
    virtual void Visit(const BinaryExpression *node) override;
    virtual void Visit(const ArrayMemberExpression *node) override;
    virtual void Visit(const ArrayLengthExpression *node) override;
    virtual void Visit(const CallMemberExpression *node) override;
    virtual void Visit(const Sequence<const IExpression> *node) override;
    virtual void Visit(const ValueExpression *node) override;
    virtual void Visit(const IdExpression *node) override;
    virtual void Visit(const ThisExpression *node) override;
    virtual void Visit(const NewIntArrayExpression *node) override;
    virtual void Visit(const NewObjectExpression *node) override;
    virtual void Visit(const NotExpression *node) override;
    virtual void Visit(const ContainerExpression *node) override;
    virtual void Visit(const Id *node) override;

private:
    struct Arrow {
        std::string From;
        std::string To;
    };

    std::vector<Arrow> arrows;
    int nodeCounter = 0;
    std::ofstream dot;
    std::string parentName;

    std::string addNode(std::string label);
    std::string format(const Coordinates &coords);
    std::string format(const T_Type& type);
    std::string format(const T_Qualifier& type);
    std::string format(const T_BinaryExpressionType& type);
    std::string format(const T_ValueType& type);
    void addArrow(std::string& name);
};

}

#endif
