#include "TinyParser.h"

TinyParser::TinyParser(std::queue<Token> &tokenQueue) : tokenQueue(tokenQueue) {}

SyntaxTreeNode* TinyParser::parseProgram()
{
    return statementList();
}

SyntaxTreeNode* TinyParser::statementList()
{
    SyntaxTreeNode* head = nullptr;
    SyntaxTreeNode* current = nullptr;

    while (!tokenQueue.empty())
    {
        if (!head)
            head = current = statement();
        else
        {
            current->children.push_back(statement());
            current = current->children.back();
        }
    }
    return head;
}

// Implement other parser methods (statement, ifStatement, etc.)
// ...
