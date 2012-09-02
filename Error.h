#ifndef _X_ERROR_H_INCLUDE_GUARD
#define _X_ERROR_H_INCLUDE_GUARD

#include <string>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>

using namespace BOOST_SPIRIT_CLASSIC_NS;

namespace {
    int error_count;
    class ErrorReportParser {
        std::string message;
    public:
        ErrorReportParser(const char* msg)
            : message(msg) {}

        typedef nil_t result_t;

        template <typename ScannerT>
        int operator()(const ScannerT& scan, result_t& result) const
        {
            throw std::runtime_error(
                '[' + boost::lexical_cast<std::string>(++error_count) + ']' + message
            );
            return 0;
        }
    };
}

typedef functor_parser<ErrorReportParser> ErrorReporter;

#endif // _X_ERROR_H_INCLUDE_GUARD

