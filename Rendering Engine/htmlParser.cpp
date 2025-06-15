#include "htmlParser.hpp"
#include <fstream>
#include <sstream>
#include<cctype>
#pragma once

HTMLParser::HTMLParser(const string& filename,bool isFile){
    if(isFile){
        ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        html = buffer.str();
    }
    else{
        html=filename;
    }
    pos=0;
};

char HTMLParser::peek(){
    if (pos >= html.size()) return '\0';
    return html[pos];
}

char HTMLParser::consume(){
    if (pos >= html.size()) return '\0';
    return html[pos++];
}

bool HTMLParser::eof(){
    return pos >= html.size();
}

void HTMLParser::skipWhitespace(){
    while((!eof()) && isspace(peek())) consume();
}

bool HTMLParser::startsWith(const string& str){
    return html.substr(pos,str.size()) == str;
}

string HTMLParser::consumeWhile(const function<bool(char)>& test){
    string result;
    while(!eof() && test(peek())){
        result += consume();
    }
    return result;
}

string HTMLParser::parseTagName(){
    skipWhitespace();
    return consumeWhile([](char c) {
        return std::isalnum(c) || c == '-' || c == '_';
    });
};

Node* HTMLParser::parseNode(){
    if(peek() == '<'){
        return parseElement();
    }
    else{
        return parseText();
    }
}

string HTMLParser::parseAttributeName(){
    return consumeWhile([](char c){
        return isalnum(static_cast<unsigned char>(c));
    });
}

string HTMLParser::parseAttributeValue(){
    skipWhitespace();
    char quote=consume();
    string value= consumeWhile([quote](char c){
        return c!=quote;
    });
    consume();
    return value;
}

vector<pair<string, string>> HTMLParser::parseAttributes() {
    vector<pair<string, string>> attrs;

    while (!eof() && peek() != '>' && peek() != '/') {
        skipWhitespace();
        string name = parseAttributeName();

        skipWhitespace();

        if (peek() != '=') {
            // Attribute without value, assume empty string
            attrs.emplace_back(name, "");
            continue;
        }

        consume(); // '='
        skipWhitespace();

        char quote = peek();
        if (quote != '\'' && quote != '"') break;
        consume(); // opening quote
        string value = consumeWhile([quote](char c) {
            return c != quote;
        });
        consume(); // closing quote
        attrs.emplace_back(name, value);
    }

    return attrs;
}


vector<Node*> HTMLParser::parseNodes(){
    vector<Node*> nodes;
    while (!eof() && !startsWith("</"))
    {
        skipWhitespace();
        nodes.push_back(parseNode());
        skipWhitespace();
    }
    return nodes;
}

ElementNode* HTMLParser::parseElement() {
    consume(); // '<'
    skipWhitespace();
    string tagName = parseTagName();
    skipWhitespace();

    auto attrs = parseAttributes();  // You must fix this too (next)
    skipWhitespace();

    bool selfClosing = false;
    if (peek() == '/') {
        consume(); // '/'
        selfClosing = true;
    }

    if (peek() == '>') consume(); // Consume '>'

    ElementNode* elem = new ElementNode(tagName);
    for (const auto& [key, value] : attrs) {
        elem->setAttribute(key, value);
    }

    if (!selfClosing) {
        auto children = parseNodes();
        for (Node* child : children) {
            elem->appendChild(child);
        }

        // --- FIX HERE --- (closing tag parsing)
        if (startsWith("</")) {
            consume(); // <
            consume(); // /
            skipWhitespace();
            string closeTag = parseTagName();
            skipWhitespace();
            if (peek() == '>') consume(); // >
        }
    }

    return elem;
}


TextNode* HTMLParser::parseText(){
    string text =consumeWhile([](char c){
        return c != '<';
    });
    return new TextNode(text);
}

Node* HTMLParser::parse(){
    auto nodes = parseNodes();
    if(nodes.size()==1) return nodes[0];

    ElementNode* root = new ElementNode("html");
    for(auto* node:nodes){
        root->appendChild(node);
    } 
    return root;
}

