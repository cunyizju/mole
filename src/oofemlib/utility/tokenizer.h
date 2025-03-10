// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef tokenizer_h
#define tokenizer_h

#include <vector>
#include <string>

#include "oofemcfg.h"

namespace oofem {
/**
 * String bracket- and quotation-aware string tokenizer.
 * This class splits given record (represented as string) to particular tokens, which are
 * separated by white spaces.
 * Tokenizer recognizes "quoted strings" and structured tokens that are
 * bounded by '{}, $$' pairs, can be nested and represent single token.
 */
class OOFEM_EXPORT Tokenizer
{
private:
    /// Array of tokens
    std :: vector< std :: string >tokens;

public:
    /// Constructor. Creates tokenizer with given character as separator.
    Tokenizer();
    /// Tokenizes given record (string).
    void tokenizeLine(const std :: string &line);
    /// returns the number of tokens.
    int giveNumberOfTokens();
    /// Returns pointer to i-th token.
    const char *giveToken(int i);
    //std::string giveToken(int i);

protected:
    /**
     * Reads next simple token (stops when whitespace character is reached)
     * @param pos Starting position.
     * @param line Record from which token is parsed.
     */
    std :: string readSimpleToken(std :: size_t &pos, const std :: string &line);
    /**
     * Reads next token (stops when separator is reached)
     * @param pos Starting position.
     * @param line Record from which token is parsed.
     * @param sep Separator.
     */
    std :: string readToken(std :: size_t &pos, const std :: string &line, char sep);
    /**
     * Reads next structured token (bounded by '{' '}' pairs, possibly nested).
     * @param pos Starting position (should point to a '{').
     * @param line Record from which token is parsed.
     */
    std :: string readStructToken(std :: size_t &pos, const std :: string &line);
    /**
     * Reads next string token (quoted).
     * @param pos Position (index) in token buffer.
     * @param line Record from which token is parsed.
     */
    std :: string readStringToken(std :: size_t &pos, const std :: string &line);
    /**
     * Reads next simple expression token (section identified by starting with '$' and finishing with '$').
     * @param pos Position (index) in token buffer.
     * @param line Record from which token is parsed.
     */
    std :: string readSimpleExpressionToken(std :: size_t &pos, const std :: string &line);
};
} // end namespace oofem
#endif // tokenizer_h
