#ifndef SYNTAXTREENODE_H
#define SYNTAXTREENODE_H

#include <string>
#include <vector>

struct Token {
    std::string value;
    std::string type;
};

struct SyntaxTreeNode {
    Token token;
    SyntaxTreeNode* next;
    std::vector<SyntaxTreeNode*> children;
    std::string shape;

    SyntaxTreeNode() : token({"", ""}), next(nullptr) {}
    SyntaxTreeNode(Token t) : token(t), next(nullptr) {}
};

#endif // SYNTAXTREENODE_H
