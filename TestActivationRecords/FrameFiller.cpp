#include "FrameFiller.h"

#include <iostream>
#include "assert.h"
#include "X86MiniJavaFrame.h"

namespace ActivationRecords {

void FrameFiller::Fill()
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
            std::unique_ptr<IFrame> frame(new X86MiniJavaFrame());
            SymbolTable::VariableInfo thisVariable("this", position, classInfo->GetTypeInfo());
            frame->AddFormal(thisVariable);
            auto argNames = methodInfo->GetArgsNames();
            for (auto name : argNames) {
                frame->AddFormal(*name);
            }
            auto localNames = methodInfo->GetVarsName();
            for(auto name : localNames) {
                frame->AddLocal(*name);
            }
            frame->AddAddressExit();
            frame->AddAddressReturnValue(methodInfo->GetReturnType().GetType());
            std::cout << methodInfo->GetFullName()->GetString() << std::endl;
            auto activation = frame->FindLocalOrFormal(StringSymbol::GetIntern("this"));
            std::cout << "this ";
            activation->print(frame->FP());
            for( auto name: argNames) {
                auto activation = frame->FindLocalOrFormal(name->GetName());
                std::cout << name->GetName()->GetString() << " ";
                activation->print(Temp(0));
            }
            std::cout << "FP: " << frame->FP().GetAddress() << std::endl;
            for( auto name: localNames) {
                auto activation = frame->FindLocalOrFormal(name->GetName());
                std::cout << name->GetName()->GetString() << " ";
                activation->print(frame->FP());
            }
            std::cout << "SP: " << frame->SP().GetAddress() << std::endl;
            std::cout << "Return address: ";
            frame->ReturnAddress()->print(Temp(0));
            std::cout << "Exit address: ";
            frame->ExitAddress()->print(Temp(0));
            std::cout << "------------------------------------------------" << std::endl;
        }
        table->FreeLastScope();
    }
    filled = true;
}

}
