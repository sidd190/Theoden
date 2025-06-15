#pragma once
#include <string>
#include <vector>

struct CSSDeclaration {
    std::string property;
    std::string value;
};

struct Specificity {
    int id = 0;
    int cls = 0;
    int tag = 0;

    bool operator<(const Specificity& other) const {
        if (id != other.id) return id < other.id;
        if (cls != other.cls) return cls < other.cls;
        return tag < other.tag;
    }
};

struct CSSRule {
    std::string selector;
    std::vector<CSSDeclaration> declarations;
    Specificity specificity;
};
