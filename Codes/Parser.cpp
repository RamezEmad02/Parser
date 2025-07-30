#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <vector>
#include <map>
#include <utility>
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <string>
#include <iostream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

// Structure to represent a token
struct Token {
    string value;
    string type;
};

/* parsing part
 ...................................................................*/
struct SyntaxTreeNode {
    Token token;
    std::string shape;  // e.g., "ellipse", "rectangle"
    std::vector<SyntaxTreeNode*> children;
    SyntaxTreeNode* next = nullptr; // For statement chaining
};



class TinyParser {
public:
    // Constructor
    TinyParser(std::queue<Token>& tokenQueue) : tokenQueue(tokenQueue) {}

    // Parse program
    SyntaxTreeNode* parseProgram() {
        return statementList();
    }

private:
    // Member variables
    queue<Token>& tokenQueue;
    SyntaxTreeNode* statementList() {
        SyntaxTreeNode* head = nullptr;
        SyntaxTreeNode* current = nullptr;

        while (!tokenQueue.empty()) {
            cout<<!head<<endl;
            if (!head) {
                head = current = statement();
            } else {
                current->next = statement();
                current = current->next;
            }

            // Consume a semicolon if present
            if (!tokenQueue.empty() && tokenQueue.front().type == "SEMICOLON") {
                match("SEMICOLON");
            }

            // Stop parsing if UNTIL or END is encountered
            if (!tokenQueue.empty() && (tokenQueue.front().type == "UNTIL" || tokenQueue.front().type == "END" ||tokenQueue.front().type == "ELSE" )) {
                break;
            }
        }
        return head;
    }

    SyntaxTreeNode* statement() {
        cout<<tokenQueue.front().type<<tokenQueue.front().value<<endl;

        if (tokenQueue.front().type == "READ") {
            return readStatement();
        }
        else if (tokenQueue.front().type == "WRITE") {
            return writeStatement();
        }
        else if (tokenQueue.front().type == "IF") {
            return ifStatement();
        }
        else if (tokenQueue.front().type == "REPEAT") {
            return repeatStatement();
        }// if there's another input not only "IDENTIFIER", put this case in else not else if
        else if (tokenQueue.front().type == "IDENTIFIER") {
            return assignmentStatement();
        }
        else {
            throw runtime_error("Token Statement Mismatch" );
        }

    }

    SyntaxTreeNode* ifStatement()
    {
        match("IF");
        SyntaxTreeNode* conditionNode = condition();
        match("THEN");
        SyntaxTreeNode* statementListNode1 = statementList();

        SyntaxTreeNode* result = new SyntaxTreeNode();
        result->token.type = "IF";
        result->token.value = "if";
        result->shape = "rectangle";
        result->children.push_back(conditionNode);
        result->children.push_back(statementListNode1);
        // Check for ELSE token and parse the second statement list if present
        if (tokenQueue.size() != 0 && tokenQueue.front().type == "ELSE")
        {
            match("ELSE");
            SyntaxTreeNode* statementListNode2 = statementList();
            result->children.push_back(statementListNode2);
        }

        match("END");
        return result;
    }

    SyntaxTreeNode* condition() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        SyntaxTreeNode* leftOperandNode = expression();

        // Check if there is a comparison operator
        if (tokenQueue.front().type == "LESSTHAN" || tokenQueue.front().type == "EQUAL") {
            string op = relationalOperator();
            SyntaxTreeNode* rightOperandNode = expression();
            result->token.type = "op";
            result->token.value = "op (" + op + ")";  // Correct formatting
            result->shape = "ellipse";
            result->children.push_back(leftOperandNode);
            result->children.push_back(rightOperandNode);
        } else {
            result = leftOperandNode;
        }

        return result;
    }

    std::string relationalOperator() {
        if (tokenQueue.empty()) {
            throw std::runtime_error("Unexpected end of input while parsing relational operator.");
        }
        std::string op;
        if (tokenQueue.front().type == "LESSTHAN" || tokenQueue.front().type == "EQUAL") {
            op = "(" + tokenQueue.front().value + ")";
            match(tokenQueue.front().type);
        } else {
            throw std::runtime_error("Invalid relational operator: " + tokenQueue.front().value);
        }
        return op;
    }

    SyntaxTreeNode* expression() {
        SyntaxTreeNode* expNode = term();
        while (tokenQueue.front().type == "PLUS" || tokenQueue.front().type == "MINUS") {
            SyntaxTreeNode* temp = new SyntaxTreeNode();
            temp->token.value = "op (" + tokenQueue.front().value + ")";  // Correct formatting
            temp->token.type = "op";
            temp->shape = "ellipse";
            match(tokenQueue.front().type);
            temp->children.push_back(expNode);
            temp->children.push_back(term());
            expNode = temp;
        }
        return expNode;
    }

    SyntaxTreeNode* term() {
        SyntaxTreeNode* factorNode = factor();
        while (tokenQueue.front().type == "MULT" || tokenQueue.front().type == "DIV") {
            SyntaxTreeNode* temp = new SyntaxTreeNode();
            temp->token.value = "op (" + tokenQueue.front().value + ")";  // Correct formatting
            temp->token.type = "op";
            temp->shape = "ellipse";
            match(tokenQueue.front().type);
            temp->children.push_back(factorNode);
            temp->children.push_back(factor());
            factorNode = temp;
        }
        return factorNode;
    }

    SyntaxTreeNode* factor() {
        SyntaxTreeNode* result = new SyntaxTreeNode();

        if (tokenQueue.front().type == "IDENTIFIER") {
            string s = tokenQueue.front().value;
            match("IDENTIFIER");
            result->token.value = "id (" + s + ")";  // Correct formatting
            result->token.type = "id";
            result->shape = "ellipse";
        }
        else if (tokenQueue.front().type == "NUMBER") {
            string s = tokenQueue.front().value;
            match("NUMBER");
            result->token.value = "const (" + s + ")";  // Correct formatting
            result->token.type = "const";
            result->shape = "ellipse";
        }
        else if (tokenQueue.front().type == "OPENBRACKET") {
            match("OPENBRACKET");
            result = condition();
            match("CLOSEDBRACKET");
        }
        else {
            throw runtime_error("InvalidFactor");
        }

        return result;
    }


    SyntaxTreeNode* repeatStatement() {
        match("REPEAT");
        SyntaxTreeNode* result = new SyntaxTreeNode();
        result->shape = "rectangle";
        result->token.type = "REPEAT";
        result->token.value = "repeat";

        SyntaxTreeNode* statementListNode = statementList();
        match("UNTIL");
        SyntaxTreeNode* conditionNode = condition();
        result->children.push_back(statementListNode);
        result->children.push_back(conditionNode);
        return result;
    }


    SyntaxTreeNode* readStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        match("READ");
        if(tokenQueue.empty())throw std::runtime_error("incomplete read statement");
        string s = tokenQueue.front().value;
        match("IDENTIFIER");

        result->token.type = "read";
        result->token.value = "read (" + s + ")";  // Correct formatting
        result->shape = "rectangle";
        return result;
    }

    SyntaxTreeNode* writeStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        match("WRITE");
        result->token.type = "WRITE";
        result->token.value = "write";
        result->shape = "rectangle";
        result->children.push_back(condition());

        return result;
    }

    SyntaxTreeNode* assignmentStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        string s = tokenQueue.front().value;
        match("IDENTIFIER");
        match("ASSIGN");
        SyntaxTreeNode* expressionNode = condition();
        result->shape = "rectangle";
        result->token.type = "assign";
        result->token.value = "assign (" + s + ")";  // Correct formatting
        result->children.push_back(expressionNode);
        return result;
    }

    // Match function to check if the current token matches the expected token
    void match(const std::string& expectedType) {
        if (tokenQueue.empty()) {
            throw std::runtime_error("Unexpected end of input: expected " + expectedType);
        }
        if (tokenQueue.front().type == expectedType) {
            tokenQueue.pop();
        }
        else {
            throw std::runtime_error("Token mismatch: expected " + expectedType + ", found " + tokenQueue.front().type);
        }
    }
};

void printSyntaxTree(SyntaxTreeNode* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    // Print the information of the current node
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";  // Adjust the indentation for better readability
    }
    std::cout << "Value: " << node->token.value << ", Type: " << node->token.type << ", Shape: " << node->shape << std::endl;

    // Recursively print the children
    for (SyntaxTreeNode* child : node->children) {
        printSyntaxTree(child, depth + 1);
    }

    // Print the next node
    if (node->next != nullptr) {
        std::cout << "Next node:" << std::endl;
        printSyntaxTree(node->next, depth);
    }
}


// qt part

struct TreeNodeVisual {
    int x;
    int y;
    string value;     // Node label with extra text
    string shape;

    TreeNodeVisual(int x_, int y_, const string& value_, const string& shape_)
        : x(x_), y(y_), value(value_), shape(shape_) {}
};

// Helper to format content
string formatContent(const string& tokenValue, const string& tokenType) {
    if (tokenType == "constant") {
        return "constant (" + tokenValue + ")";
    } else if (tokenType == "identifier") {
        return "variable (" + tokenValue + ")";
    } else if (tokenType == "keyword" && tokenValue == "read") {
        return "read (" + tokenValue + ")";
    } else if (tokenType == "keyword" && tokenValue == "write") {
        return "write (" + tokenValue + ")";
    }
    return tokenValue;
}

// Traverse the tree and calculate positions dynamically
int traverseTree(SyntaxTreeNode* node, int depth, int& xPosition,
                 vector<TreeNodeVisual>& nodes,
                 vector<pair<int, int>>& edges,
                 int& nodeCounter,
                 map<SyntaxTreeNode*, int>& nodeIndices) {
    if (!node) return 0;

    int currentNodeIndex = nodeCounter++;


    // Add current node to the visualization
    string nodeLabel = node->token.value.empty() ? "Unknown" : node->token.value;
    nodes.emplace_back(xPosition * 150, depth * 150, nodeLabel, node->shape);
    nodeIndices[node] = currentNodeIndex;

    int maxChildWidth = 0; // Track the width of child subtrees
    int currentChildX = xPosition;

    // Traverse children
    for (auto* child : node->children) {
        int childWidth = traverseTree(child, depth + 1, currentChildX, nodes, edges, nodeCounter, nodeIndices);
        edges.emplace_back(currentNodeIndex, nodeIndices[child]);  // Parent -> Child
        currentChildX += childWidth; // Adjust position based on subtree size
        maxChildWidth = std::max(maxChildWidth, childWidth);
    }

    // Adjust parent node's position to be centered relative to its children
    if (!node->children.empty()) {
        int centerX = (xPosition + currentChildX - 1) / 2; // Center of subtree
        nodes[currentNodeIndex].x = centerX * 150;
        xPosition = currentChildX; // Update xPosition for next sibling
    } else {
        xPosition++; // No children, just move to the right
    }

    // Handle sibling nodes
    if (node->next) {
        int siblingX = xPosition;
        int siblingWidth = traverseTree(node->next, depth, siblingX, nodes, edges, nodeCounter, nodeIndices);
        edges.emplace_back(currentNodeIndex, nodeIndices[node->next]);
        xPosition = siblingX;
        maxChildWidth = std::max(maxChildWidth, siblingWidth);
    }

    return std::max(1, maxChildWidth); // Return subtree width
}



void visualizeTree(SyntaxTreeNode* root) {
    // Create a dialog for visualization
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("Syntax Tree Visualization");
    dialog->resize(1200, 800);

    // Create a layout for the dialog
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Create the scene and view for the syntax tree
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    layout->addWidget(view);

    // Create the OK button
    QPushButton* okButton = new QPushButton("OK");
    QObject::connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(okButton);

    // Tree visualization logic
    vector<TreeNodeVisual> nodes;
    vector<pair<int, int>> edges;  // Edges between nodes
    map<SyntaxTreeNode*, int> nodeIndices;
    int nodeCounter = 0;
    int xPosition = 0;

    // Traverse tree to calculate positions and edges
    traverseTree(root, 0, xPosition, nodes, edges, nodeCounter, nodeIndices);

    // Draw edges
    for (const auto& edge : edges) {
        const auto& parent = nodes[edge.first];
        const auto& child = nodes[edge.second];
        scene->addLine(parent.x + 50, parent.y + 25, child.x + 50, child.y + 25, QPen(Qt::black));
    }

    // Draw nodes with shapes and formatted text
    for (const auto& node : nodes) {
        QGraphicsItem* shapeItem;

        if (node.shape == "ellipse") {
            shapeItem = scene->addEllipse(node.x, node.y, 100, 50, QPen(Qt::black), QBrush(Qt::white));
        } else { // Default to rectangle
            shapeItem = scene->addRect(node.x, node.y, 100, 50, QPen(Qt::black), QBrush(Qt::white));
        }

        QGraphicsTextItem* text = scene->addText(QString::fromStdString(node.value));
        text->setDefaultTextColor(Qt::black);
        QRectF textRect = text->boundingRect();
        text->setPos(node.x + 50 - textRect.width() / 2, node.y + 25 - textRect.height() / 2);
        text->setParentItem(shapeItem);

        // Special handling for "else" nodes
        if (node.value == "else") {

            QGraphicsTextItem* label = scene->addText("ELSE");
            label->setDefaultTextColor(Qt::red);
            QRectF labelRect = label->boundingRect();
            label->setPos(node.x + 50 - labelRect.width() / 2, node.y - labelRect.height());
        }
    }

    // Show the dialog
    dialog->exec();

    // Clean up
    delete scene;
}



Token tokenizeLine(const string& line) {

    istringstream iss(line);
    string temp;
    Token token;
    while (getline(iss, temp, ',')) {
        // Assuming the format is always "value, type"
        getline(iss, token.type, '\n');
        token.value = temp;
    }

    return token;
}

int tree(QString text2) {

    string codeSnippet = text2.toStdString();
    stringstream str(codeSnippet);
    vector<Token> tokens;
    try {

        queue<Token> tokenQueue;
        string line;
        while (getline(str, line)) {
            Token lineTokens = tokenizeLine(line);
            tokenQueue.push(lineTokens);
        }
        TinyParser parser(tokenQueue);
        SyntaxTreeNode* root = parser.parseProgram();

        printSyntaxTree(root); // Print tree to console
        visualizeTree(root); // Visualize tree using Qt
    } catch (const exception& e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}
