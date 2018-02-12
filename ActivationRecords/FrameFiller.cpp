#include "FrameFiller.h"

#include <iostream>
#include "assert.h"
#include "X86MiniJavaFrame.h"
#include "NameConventions.h"

namespace ActivationRecords {

void FrameFiller::PrintFill()
{
    assert(!filled);
    auto classNames = table->GetClassesNames();
    for(auto className : classNames) {
        SymbolTable::Position position;
        table->AddClassToScope(className->GetString(), position);
        auto classInfo = table->GetClass(className->GetString(), position);
        auto methods = classInfo->GetMethodNames();
        for(auto method: methods) {
            auto methodInfo = table->GetMethod(method->GetString(), position);
            std::unique_ptr<IFrame> frame(CreateFrame(*classInfo, *methodInfo));
            std::cout << methodInfo->GetFullName()->GetString() << std::endl;
            auto activation = frame->FindLocalOrFormal(StringSymbol::GetIntern("this"));
            std::cout << "this ";
            activation->print(frame->FP());
            for( auto name: methodInfo->GetArgs()) {
                auto activation = frame->FindLocalOrFormal(name->GetName());
                std::cout << name->GetName()->GetString() << " ";
                activation->print(TempAddress(0));
            }
            std::cout << "FP: " << frame->FP().GetAddress() << std::endl;
            for( auto name: methodInfo->GetVars()) {
                auto activation = frame->FindLocalOrFormal(name->GetName());
                std::cout << name->GetName()->GetString() << " ";
                activation->print(frame->FP());
            }
            std::cout << "SP: " << frame->SP().GetAddress() << std::endl;
            std::cout << "Return address: ";
            frame->ReturnAddress()->print(TempAddress(0));
            std::cout << "Exit address: ";
            frame->ExitAddress()->print(TempAddress(0));
            std::cout << "------------------------------------------------" << std::endl;
        }
        table->FreeLastScope();
    }
    filled = true;
}

IFrame* FrameFiller::CreateFrame(const SymbolTable::ClassInfo& classInfo, const SymbolTable::MethodInfo& methodInfo)
{
    std::unique_ptr<IFrame> frame(Factory::Create<IFrame>());
    SymbolTable::Position position;
    SymbolTable::VariableInfo thisVariable(NameConventions::ThisName, position, classInfo.GetTypeInfo());
    frame->AddFormal(thisVariable);
    auto argNames = methodInfo.GetArgs();
    for (auto name : argNames) {
        frame->AddFormal(*name);
    }
    auto localNames = methodInfo.GetVars();
    for(auto name : localNames) {
        frame->AddLocal(*name);
    }
    frame->AddAddressExit();
    frame->AddAddressReturnValue(methodInfo.GetReturnType().GetType());
    return frame.release();
}

}
