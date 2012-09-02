#include "Syntax.h"
#include "XAssert.h"

Data::SubTable Semantics::subs;
Data::Subroutine* Semantics::current;

void Semantics::goUp(char c)
{
    X_ASSERT(c == ')');
    current = current->getParent();
}

void Semantics::addSub(IteratorType begin, IteratorType end)
{
    std::string name = std::string(begin, end);
    Data::Subroutine* sub = new Data::Subroutine(current, name);
    subs.push_back(sub);
    current = sub;
}

void Semantics::addInt(int n)
{
    current->addInstruction(new Data::Instruction(n));
}

void Semantics::addReal(double n)
{
    current->addInstruction(new Data::Instruction(n));
}

void Semantics::addChar(char c)
{
    current->addInstruction(new Data::Instruction(c));
}

void Semantics::addString(IteratorType begin, IteratorType end)
{
    std::string value = std::string(begin, end);
    current->addInstruction(new Data::Instruction(value));
}

void Semantics::addCall(IteratorType begin, IteratorType end)
{
    std::string value = std::string(begin, end);
    current->addInstruction(new Data::Instruction(value, true));
}

Data::SubTable& Semantics::getRoutines()
{
    return subs;
}

void Semantics::cleanUp()
{
    for(Data::Subroutine* sub : subs) {
        if(sub != nullptr)
            delete sub;
    }
}

Syntax::Syntax()
{
}
