#pragma once
#include "dom.hpp"
#include<functional>
#include <string>

class HTMLParser{
    public:
        HTMLParser(const string& filename,bool isFile);
        Node* parse();

    private:
        string html;
        size_t pos;

        void skipWhitespace();
        bool startsWith(const string& str);
        bool eof();
        char peek();
        char consume();
        string consumeWhile(const function<bool(char)>& test);
        vector<pair<string, string>> parseAttributes();
        string parseTagName();
        string parseAttributeName();
        string parseAttributeValue();
        void parseAttributes(ElementNode* elem);
        Node* parseNode();
        ElementNode* parseElement();
        TextNode* parseText();
        vector<Node*> parseNodes();
};