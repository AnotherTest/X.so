#define _X_DEBUGMODE

#include <iostream>
#include <string>

#include "Interpreter.h"
#include "Syntax.h"

bool parseFile(const std::string& name, const Syntax& parser)
{
    IteratorType begin(name);
    if(!begin) {
        std::cerr << "Unable to open file!" << std::endl;
        return false;
    }
    IteratorType end = begin.make_end();
    return doParse(begin, end, parser);
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cerr << "Usage: ./X <filename>" << std::endl;
        return 1;
    }
    Syntax parser;
    bool success = parseFile(argv[1], parser);
    if(!success) {
        std::cerr << "Errors while parsing." << std::endl;
        return 2;
    }
    ModuleLoader mod_loader;
    Data::XStack stack;
    Interpreter x(Semantics::getRoutines(), mod_loader, stack);
    x.interpret();
    Semantics::cleanUp();
}
