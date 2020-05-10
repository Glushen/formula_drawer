#include "formula_drawer.h"
#include "expression.h"
#include <cstdarg>
#include <parser.h>

extern void yy_set_input_string(const char*);
extern void yy_clear_buffer();

static fd::Result result;
static std::string fileName;

fd::Result fd::drawExpression(const std::string& inputExpression, const std::string& outputFileName) {
#if YYDEBUG
    yydebug = 1;
#endif

    fileName = outputFileName;

    yy_set_input_string(inputExpression.c_str());
    yyparse();
    yy_clear_buffer();

    return result;
}

void yy_accept_ast(std::unique_ptr<fd::exp::Expression> expression) {
    result.accepted = true;

    auto view = expression->createView();
    view->measure();
    view->layout();

    auto image = QImage(view->w, view->h, QImage::Format_RGB32);
    image.fill(QColor(255, 255, 255));
    auto painter = QPainter(&image);
    auto pen = QPen(QColor(0, 0, 0));
    pen.setWidthF(3);
    painter.setPen(pen);
    view->draw(painter);
    image.save(fileName.c_str(), "PNG", 100);
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
