#include <iostream>
#include<algorithm>
#include<vector>
using namespace std;

enum class NodeType{
    ELEMENT,
    TEXT
};

class Node{
    public:
        NodeType type;
        vector<Node*> children; 
        Node* parent = nullptr;

        Node(NodeType t):type(t) {}

        virtual ~Node(){
            for(auto child:children) 
                delete child;
        }

        void appendChild(Node* child){
            child -> parent = this;
            children.push_back(child);
        }
};

struct Attribute {
    string name;
    string value;
    Attribute (const string& n,const string& v):name(n) , value(v) {}
};

class ElementNode : public Node {
    public :
        string tagName;
        vector<Attribute> attributes;

        ElementNode(const string& tag):Node(NodeType::ELEMENT),tagName(tag) {}

        void setAttribute(const string& name, const string& value){
            for(auto& attr:attributes){
                if(attr.name == name){
                    attr.value = value;
                   return;
                }
            }
            attributes.emplace_back(name,value);
        }

        string getAttribute(const string& name) const{
            for(const auto& attr:attributes){
                if(attr.name==name){
                    return attr.value;
                }
                return "";
            }
        }
};

class TextNode :public Node{
    public:
        string text;
        TextNode(const string& txt):Node(NodeType::TEXT),text(txt) {}
};

void printDOM(const Node* node, int depth = 0) {
    string indent(depth * 2, ' ');

    if (node->type == NodeType::ELEMENT) {
        const ElementNode* elem = static_cast<const ElementNode*>(node);

        // Opening tag
        cout << indent << "<" << elem->tagName;
        if (!elem->attributes.empty()) {
            for (const auto& attr : elem->attributes) {
                cout << " " << attr.name << "=\"" << attr.value << "\"";
            }
        }
        cout << ">" << endl;

        // Children
        for (const Node* child : elem->children) {
            printDOM(child, depth + 1);
        }

        // Closing tag
        cout << indent << "</" << elem->tagName << ">" << endl;
    }

    else if (node->type == NodeType::TEXT) {
        const TextNode* text = static_cast<const TextNode*>(node);

        // to Trim text content
        string trimmed;
        for (char c : text->text) {
            if (c != '\n' && c != '\r') trimmed += c;
        }

        // to print only non-empty, non-whitespace-only text nodes
        if (!all_of(trimmed.begin(), trimmed.end(), ::isspace) && !trimmed.empty()) {
            cout << indent << trimmed << endl;
        }
    }
}
