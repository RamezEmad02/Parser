#ifndef TINYPARSER_H
#define TINYPARSER_H

#include <queue>
#include <string>
#include <vector>
#include <stdexcept>

struct Token
{
    std::string value;
    std::string type;
};

struct SyntaxTreeNode
{
    Token token;
    std::vector<SyntaxTreeNode*> children;
    std::string shape;

    SyntaxTreeNode(Token t = {"", ""}) : token(t) {}
};

class TinyParser
{
public:
    TinyParser(std::queue<Token> &tokenQueue);
    SyntaxTreeNode* parseProgram();

private:
    std::queue<Token> &tokenQueue;

    SyntaxTreeNode* statementList();
    SyntaxTreeNode* statement();
    SyntaxTreeNode* ifStatement();
    SyntaxTreeNode* condition();
    std::string relationalOperator();
    SyntaxTreeNode* expression();
    SyntaxTreeNode* term();
    SyntaxTreeNode* factor();
    SyntaxTreeNode* repeatStatement();
    SyntaxTreeNode* readStatement();
    SyntaxTreeNode* writeStatement();
    SyntaxTreeNode* assignmentStatement();
    void match(const std::string &expectedType);
};

#endif // TINYPARSER_H
