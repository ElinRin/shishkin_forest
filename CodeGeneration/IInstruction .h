#include "CTempList.h
namespace CodeGeneration {


    class IInstruction {
    public:
        std::string AsmCode;

        const IR::CTempList* UsedVars() const {
            return src;
        };
        const IR::CTempList* DefindedVars() const {
            return dst;
        };
        const IR::CLabelList* JumpTargets() const {
            return labelList;
        };
        std::string Format( const IR::CTempMap* varsMapping ) const;

        IInstruction( const std::string& a, const IR::CTempList& s, const IR::CTempList& d,
            const IR::CLabelList& l ) :
                AsmCode(a),
                src(s),
                dst(d),
                labelList(l) {}

        virtual ~IInstruction() = 0 {}

    protected:
        IR::CTempList src;
        IR::CTempList dst;
        IR::CLabelList* labelList;
    };

    class COper : public IInstruction {
    };

    class CMove : public IInstruction {
    public:
        CMove( const std::string& a, const IR::CTempList& s, const IR::CTempList& d) :
                AsmCode(a),
                src(s),
                dst(d) {}
    };

    class CLabel : public IInstruction {
    public:
        CLabel( const IR::CLabelList& l ) :
                labelList(l) {}

    };

}