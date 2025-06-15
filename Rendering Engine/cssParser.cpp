#include "cssParser.hpp"
#include <cctype>

CSSParser::CSSParser(const std::string& input) : css(input), pos(0) {}

char CSSParser::peek() { return css[pos]; }
char CSSParser::consume() { return css[pos++]; }
bool CSSParser::eof() { return pos >= css.size(); }

void CSSParser::skipWhitespace() {
    while (!eof() && std::isspace(peek())) consume();
}

std::string CSSParser::consumeWhile(bool (*test)(char)) {
    std::string result;
    while (!eof() && test(peek())) result += consume();
    return result;
}

std::string CSSParser::consumeIdentifier() {
    skipWhitespace();
    return consumeWhile([](char c) {
        return std::isalnum(c) || c == '-' || c == '_' || c == '#' || c == '.';
    });
}

std::string CSSParser::consumeUntil(char delimiter) {
    std::string result;
    while (!eof() && peek() != delimiter) result += consume();
    return result;
}

Specificity CSSParser::computeSpecificity(const std::string& selector) {
    Specificity spec;
    size_t i = 0;
    while (i < selector.size()) {
        if (selector[i] == '#') {
            ++spec.id;
            ++i;
            while (i < selector.size() && std::isalnum(selector[i])) ++i;
        } else if (selector[i] == '.') {
            ++spec.cls;
            ++i;
            while (i < selector.size() && std::isalnum(selector[i])) ++i;
        } else if (std::isalpha(selector[i])) {
            ++spec.tag;
            while (i < selector.size() && std::isalnum(selector[i])) ++i;
        } else {
            ++i;
        }
    }
    return spec;
}

std::vector<CSSDeclaration> CSSParser::parseDeclarations() {
    std::vector<CSSDeclaration> declarations;
    skipWhitespace();
    if (eof() || peek() != '{') return declarations;
    consume();  // consume '{'

    while (!eof()) {
        skipWhitespace();
        if (peek() == '}') {
            consume();  // consume '}'
            break;
        }

        size_t start = pos;
        std::string property = consumeIdentifier();
        skipWhitespace();

        if (eof() || consume() != ':') {
            // Invalid declaration, skip to next semicolon or closing brace
            while (!eof() && peek() != ';' && peek() != '}') consume();
            if (peek() == ';') consume();  // Consume ';' if found
            continue;
        }

        skipWhitespace();
        std::string value = consumeUntil(';');
        if (eof()) break;

        // Sanity check for non-empty value
        if (!property.empty() && !value.empty()) {
            declarations.push_back({property, value});
        }

        if (!eof() && peek() == ';') consume();  // consume ';'
    }

    return declarations;
}


std::vector<CSSRule> CSSParser::parse() {
    std::vector<CSSRule> rules;
    while (!eof()) {
        skipWhitespace();
        std::string selector = consumeIdentifier();
        Specificity spec = computeSpecificity(selector);
        std::vector<CSSDeclaration> declarations = parseDeclarations();
        rules.push_back({selector, declarations, spec});
    }
    return rules;
}
