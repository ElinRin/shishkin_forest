#include <Linerizer.h>

namespace IRTranslate {

void Linerizer::Linerize(ISubtreeWrapper* wrapper, std::vector<std::unique_ptr<IStm> >& stms)
{
    assert(statements == nullptr);
    isPreviousDetached = false;
    statements = &stms;
    wrapper->AcceptVisitor(this);
    statements = nullptr;
}

void Linerizer::Visit(Seq* node)
{
    if(node->Head.get() != nullptr) {
        node->Head->AcceptVisitor(this);
        if(isPreviousDetached) {
            isPreviousDetached = false;
            node->Head.release();
        }
    }
    if(node->Tail.get() != nullptr) {
        node->Tail->AcceptVisitor(this);
        if(isPreviousDetached) {
            isPreviousDetached = false;
            node->Tail.release();
        }
    }
}

void Linerizer::Visit(StmWrapper* node)
{
    std::unique_ptr<IStm> stm(node->ToStm());
    stm->AcceptVisitor(this);
}

void Linerizer::Visit(ExpWrapper* node)
{
    std::unique_ptr<IStm> stm(node->ToStm());
    stm->AcceptVisitor(this);
}

void Linerizer::addToStatements(IStm* stm)
{
    statements->push_back(std::unique_ptr<IStm>(stm));
    isPreviousDetached = true;
}

}
