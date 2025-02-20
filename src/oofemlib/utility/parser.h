// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef parser_h
#define parser_h

#include "oofemcfg.h"

namespace oofem {
#define Parser_CMD_LENGTH 1024
#define Parser_TBLSZ 23

/**
 * Class for evaluating mathematical expressions in strings.
 * Strings should be in MATLAB syntax. The parser understands variable names with values set by "x=expression;"
 * The following binary operators are recognized: < > == + - * /
 * and the following functions: sqrt, sin, cos, tan, atan, asin, acos, exp,
 * as well as parentheses ( )
 *
 * Example string:
 * x=3;y=7;sqrt(x*(x/y+3))
 */
class OOFEM_EXPORT Parser
{
public:
    Parser() {
        curr_tok = PRINT;
        no_of_errors = 0;
        for ( int i = 0; i < Parser_TBLSZ; i++ ) {
            table [ i ] = 0;
        }
    }
    ~Parser() {
        reset();
    }

    double eval(const char *string, int &err);
    void   reset();

private:
    enum Token_value {
        NAME, NUMBER, END,
        SQRT_FUNC, SIN_FUNC, COS_FUNC, TAN_FUNC, ATAN_FUNC, ASIN_FUNC, ACOS_FUNC, EXP_FUNC, INT_FUNC, HEAVISIDE_FUNC, HEAVISIDE_FUNC1,
        PLUS='+', MINUS='-', MUL='*', DIV='/', MOD='%', POW='^', BOOL_EQ, BOOL_LE, BOOL_LT, BOOL_GE, BOOL_GT,
        PRINT=';', ASSIGN='=', LP='(', RP=')'
    };

    int no_of_errors;
    Token_value curr_tok;
    struct name {
        char *string;
        name *next;
        double value;
    };
    name *table [ Parser_TBLSZ ];
    double number_value;
    char string_value [ Parser_CMD_LENGTH ];
    const char *parsedLine;

    name *look(const char *p, int ins = 0);
    inline name *insert(const char *s) { return look(s, 1); }
    void error(const char *s);
    double expr(bool get);
    double term(bool get);
    double prim(bool get);
    double agr(bool get);
    Token_value get_token();
};
} // end namespace oofem
#endif // parser_h
