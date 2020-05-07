project(formula_drawer_lib)

find_package(BISON)
find_package(FLEX)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

bison_target(
    parser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
    VERBOSE
)
flex_target(
    lexer
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)
add_flex_bison_dependency(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
    formula_drawer_lib STATIC
    formula_drawer.cpp formula_drawer.h
    parser_helper.h parser_helper.cpp
    expression.cpp expression.h
    view.h view.cpp
    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)

target_link_libraries(formula_drawer_lib Qt5::Widgets)
