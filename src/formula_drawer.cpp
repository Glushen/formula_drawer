#include "formula_drawer.h"
#include "expression.h"
#include <cstdarg>
#include <parser.h>

extern void yy_set_input_string(const char*);
extern void yy_clear_buffer();

static fd::Result result;

fd::Result fd::drawExpression(const std::string& inputExpression, const std::string& outputFileName) {
#if YYDEBUG
    yydebug = 1;
#endif

    yy_set_input_string(inputExpression.c_str());
    yyparse();
    yy_clear_buffer();

    return result;
}

void yy_accept_ast(fd::exp::Expression expression) {
    result.accepted = true;
    // TODO
}

void yyerror(const char* format, ...) {
    result.accepted = false;

    va_list arguments;

    va_start(arguments, format);
    char* result_chars = new char[vsnprintf(nullptr, 0, format, arguments) + 1];
    va_end(arguments);

    va_start(arguments, format);
    vsprintf(result_chars, format, arguments);
    va_end(arguments);

    result.errorMessage = result_chars;
    delete[] result_chars;
}
