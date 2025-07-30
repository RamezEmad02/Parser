#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <queue> // Add this line

struct Token
{
    std::string value;
    std::string type;
};

struct SyntaxTreeNode
{
    Token token;
    SyntaxTreeNode* next;
    std::vector<SyntaxTreeNode*> children;
    std::string shape;

    SyntaxTreeNode() : token({"", ""}), next(nullptr) {}
    SyntaxTreeNode(Token t) : token(t), next(nullptr) {}
};

class TinyParser
{
public:
    explicit TinyParser(std::queue<Token>& tokenQueue);
    SyntaxTreeNode* parseProgram();

private:
    std::queue<Token>& tokenQueue;

    SyntaxTreeNode* statementList();
    SyntaxTreeNode* statement();
    SyntaxTreeNode* ifStatement();
    SyntaxTreeNode* condition();
    SyntaxTreeNode* expression();
    SyntaxTreeNode* term();
    SyntaxTreeNode* factor();
    SyntaxTreeNode* repeatStatement();
    SyntaxTreeNode* readStatement();
    SyntaxTreeNode* writeStatement();
    SyntaxTreeNode* assignmentStatement();
    std::string relationalOperator();
    void match(std::string expectedType);
};

Token tokenizeLine(const std::string& line);

#endif // PARSER_H
