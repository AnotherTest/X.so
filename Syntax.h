#ifndef _X_SYNTAX_H_INCLUDE_GUARD
#define _X_SYNTAX_H_INCLUDE_GUARD

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>

#include "Error.h"
#include "Data.h"

using namespace BOOST_SPIRIT_CLASSIC_NS;

typedef file_iterator<char> IteratorType;

namespace Semantics {
    extern Data::SubTable subs;
    extern Data::Subroutine* current;

    /**
     * @pre c == ')'
     */
    void goUp(char c);

    void addSub(IteratorType begin, IteratorType end);

    void addInt(int n);
    void addReal(double n);
    void addChar(char c);
    void addString(IteratorType begin, IteratorType end);
    void addCall(IteratorType begin, IteratorType end);

    Data::SubTable& getRoutines();
    void cleanUp();
};

struct Syntax : public grammar<Syntax> {
    static ErrorReporter expect_lparen;
    static ErrorReporter expect_call;
    static ErrorReporter expect_id;
    Syntax();

    template <typename ScannerT>
    struct definition {
        rule<ScannerT> id, string, character, call, literal,
                       stmnt, body, block, program;

        definition(const Syntax& self)
            : id(), string(), character(), call(), literal(),
              stmnt(), body(), block(), program()
        {
            id =
                lexeme_d[ (alpha_p | '_') >> *( alnum_p | '_' ) ]
                ;
            string =
                lexeme_d[ '"' >> (*(anychar_p - '"'))[&Semantics::addString] >> '"' ]
                ;
            character =
                lexeme_d[ '\'' >> anychar_p[&Semantics::addChar] >> '\'' ]
                ;
            call =
                id >> !('.' >> (call | expect_call))
                ;
            literal =
                int_p[&Semantics::addInt] | real_p[&Semantics::addReal] | string
                | character | call[&Semantics::addCall]
                ;
            stmnt =
                !((literal >> stmnt) | (call >> stmnt))
                ;
            body =
                (block >> body) | stmnt
                ;
            block =
                '$' >> (id[&Semantics::addSub] | expect_id) >> ('(' | expect_lparen) >> body
                    >> ch_p(')')[&Semantics::goUp]
                ;
            program =
                block >> end_p
                ;
        }

        const rule<ScannerT>& start() const
        {
            return program;
        }
    };
};

ErrorReporter Syntax::expect_lparen("expecting left parenthese");
ErrorReporter Syntax::expect_call("expecting call");
ErrorReporter Syntax::expect_id("expecting identifier");

template <typename SyntaxT>
bool doParse(IteratorType begin, IteratorType end, const SyntaxT& grammar)
{
    try {
        auto info = parse(begin, end, grammar, space_p);
        return info.full;
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

}

#endif // _X_SYNTAX_H_INCLUDE_GUARD

