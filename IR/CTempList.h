#include <string>

namespace IR
{
    class CTemp {
    public:
        CTemp();
        CTemp( const CTemp& src ) : name( src.name ) {}

        const std::string& GetName() const {
            return name;
        }

        bool operator == (const CTemp& src) const {
            return name == src.name;
        }
        bool operator != (const CTemp& src) const {
            return !this->operator==(src);
        }

    private:
        std::string name;
    };

    class CLabel {
    public:
        CLabel();
        CLabel( const CLabel& src ) : name( src.name ) {}

        const std::string& GetName() const {
            return name;
        }

        bool operator == (const CLabel& src) const {
            return name == src.name;
        }
        bool operator != (const CLabel& src) const {
            return !this->operator==(src);
        }

    private:
        std::string name;
    };

    class CTempList {
    public:
        CTempList();

    private:
        std::list< CTemp> tempList;
    };

    class CLabelList {
    public:
        CLabelList();

    private:
        std::list< CTemp> labelList;
    };

    class CTempMap {
    public:
        CTempMap();
    private:
        std::unordered_map<Temp::CTemp, std::string> tempMap;
    };

}
