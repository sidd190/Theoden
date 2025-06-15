#pragma once
#include "css.hpp"
#include <string>
#include <vector>

class CSSParser {
public:
    CSSParser(const std::string& input);

    std::vector<CSSRule> parse();

private:
    std::string css;
    size_t pos;

    char peek();
    char consume();
    bool eof();
    void skipWhitespace();

    std::string consumeWhile(bool (*test)(char));
    std::string consumeIdentifier();
    std::string consumeUntil(char delimiter);
    Specificity computeSpecificity(const std::string& selector);
    std::vector<CSSDeclaration> parseDeclarations();
};
