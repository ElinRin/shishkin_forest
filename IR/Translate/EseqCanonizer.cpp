#include <EseqCanonizer.h>

namespace IRTranslate {

StmWrapper* EseqCanonizer::Canonize(ISubtreeWrapper* wrapper)
{
    lastEseq.reset(new Eseq(nullptr, nullptr));
    wrapper->AcceptVisitor(this);
    return new StmWrapper(lastEseq->Statement.release());
}

void EseqCanonizer::Visit(IR::Unaryop* node)
{
    lastEseq->Statement.reset(reorder(node->Expression));
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Binop* node)
{
    IStm* leftStatements = reorder(node->LeftExpression);
    IStm* rightStatements = reorder(node->RightExpression);
    lastEseq->Statement.reset(new Seq(leftStatements, rightStatements));
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Call* node)
{
    node->Arguments->AcceptVisitor(this);
    node->Arguments.release();
    node->Arguments.reset(static_cast<ExpList*>(lastEseq->Expression.release()));
    lastEseq->Statement.reset(reorder(node->FuncExpression));
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Eseq* node)
{
    node->Statement->AcceptVisitor(this);
    node->Statement.release();
    lastEseq->Statement.reset(reorder(node->Expression));
    lastEseq->Expression.reset(node->Expression.release());
    delete node;
}

void EseqCanonizer::Visit(Mem* node)
{
    node->Expression->AcceptVisitor(this);
    node->Expression.release();
    node->Expression.reset(lastEseq->Expression.release());
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Name* node)
{
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Temp* node)
{
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(Exp* node)
{
    IStm* stm = reorder(node->expression);
    if(stm != nullptr) {
        lastEseq->Statement.reset(addSeqIfRequired(stm));
    }
    lastEseq->Statement.reset(addSeqIfRequired(node));
}

void EseqCanonizer::Visit(Jump* node)
{
    lastEseq->Statement.reset(addSeqIfRequired(node));
}

void EseqCanonizer::Visit(JumpC* node)
{
    IStm* leftStatements = reorder(node->ConditionLeftExpression);
    IStm* rightStatements = reorder(node->ConditionRightExpression);
    if(leftStatements != nullptr) {
        lastEseq->Statement.reset(addSeqIfRequired(leftStatements));
    }
    if(rightStatements != nullptr) {
        lastEseq->Statement.reset(addSeqIfRequired(rightStatements));
    }
    lastEseq->Statement.reset(addSeqIfRequired(node));
}

void EseqCanonizer::Visit(LabelStm* node)
{
    lastEseq->Statement.reset(addSeqIfRequired(node));
}

void EseqCanonizer::Visit(Move* node)
{
    IStm* src = reorder(node->Source);
    IStm* dst = reorder(node->Destination);
    if(src != nullptr) {
        lastEseq->Statement.reset(addSeqIfRequired(src));
    }
    if(dst != nullptr) {
        lastEseq->Statement.reset(addSeqIfRequired(dst));
    }
    lastEseq->Statement.reset(addSeqIfRequired(node));
}

void EseqCanonizer::Visit(Seq* node)
{

    node->Head->AcceptVisitor(this);
    node->Head.release();
    if(node->Tail != nullptr) {
        node->Tail->AcceptVisitor(this);
    }
    node->Tail.release();
    delete node;
}

void EseqCanonizer::Visit(ExpList* node)
{
    if(node->Head != nullptr) {
        IStm* stm = reorder(node->Head);
        if(stm != nullptr) {
            lastEseq->Statement.reset(addSeqIfRequired(stm));
        }
    }
    if(node->Tail != nullptr) {
        node->Tail->AcceptVisitor(this);
        node->Tail.release();
        node->Tail.reset(lastEseq->Expression.release());
    }
    lastEseq->Expression.reset(node);
}

void EseqCanonizer::Visit(StmWrapper* node)
{
    node->ToStm()->AcceptVisitor(this);
}

void EseqCanonizer::Visit(ExpWrapper* node)
{
    node->ToStm()->AcceptVisitor(this);
}

IStm* EseqCanonizer::reorder(std::unique_ptr<IExp>& exp)
{
    exp->AcceptVisitor(this);
    exp.release();
    decomposeEseq();
    exp.reset(lastEseq->Expression.release());
    return lastEseq->Statement.release();
}

IStm*EseqCanonizer::addSeqIfRequired(IStm* stm)
{
    assert(stm != nullptr);
    if(lastEseq->Statement == nullptr) {
        return stm;
    }
    return new Seq(lastEseq->Statement.release(), stm);
}

void EseqCanonizer::decomposeEseq()
{
    if(lastEseq->Statement.get() == nullptr || lastEseq->Expression.get() == nullptr) {
        return;
    }
    if(lastEseq->Expression->IsCommutative()) {
        return;
    } else {
        Temp* holder = new Temp(Temp::TempHolderLocalId);
        lastEseq->Statement.reset(new Seq(lastEseq->Statement.release(),
                                          new Move(holder, lastEseq->Expression.release())));
        lastEseq->Expression.reset(new Mem(new Temp(*holder)));
    }
}

}
