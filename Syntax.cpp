#include "Syntax.h"
#include "XAssert.h"

Data::SubTable Semantics::subs;
Data::Subroutine* Semantics::current;

void Semantics::parseEscape(std::string& s, const std::string& escp, const std::string& repl)
{
    size_t pos = 0;
    while((pos = s.find(escp, pos)) != std::string::npos) {
        if(s[pos - 1] == '\\') {
            pos += escp.length();
            continue;
        }
        s.replace(pos, escp.length(), repl);
    }
}

void Semantics::parseEscapes(std::string& s)
{
    parseEscape(s, "\\n", "\n");
    parseEscape(s, "\\s", " ");
    parseEscape(s, "\\t", "\t");
    parseEscape(s, "\\v", "\v");
    parseEscape(s, "\\b", "\b");
}

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
    parseEscapes(value);
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
