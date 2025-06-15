#include "htmlParser.cpp"

int main() {

    HTMLParser parser("example.html",true);
    Node* root = parser.parse();

    printDOM(root);  // from dom.hpp

    delete root;
    return 0;
}
