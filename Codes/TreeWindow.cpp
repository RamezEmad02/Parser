#include "treewindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

TreeWindow::TreeWindow(QWidget *parent)
    : QWidget(parent), scene(new QGraphicsScene(this))
{
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setGeometry(10, 10, 800, 600);
    setFixedSize(820, 620);
}

void TreeWindow::setSyntaxTree(SyntaxTreeNode *root)
{
    scene->clear();
    if (root)
        drawTree(root, 400, 50, 200);
}

void TreeWindow::drawTree(SyntaxTreeNode *node, int x, int y, int xOffset)
{
    if (!node) return;

    QGraphicsItem *item;
    if (node->shape == "ellipse")
        item = scene->addEllipse(x - 20, y - 20, 40, 40);
    else
        item = scene->addRect(x - 20, y - 20, 40, 40);

    scene->addText(QString::fromStdString(node->token.value))->setPos(x - 15, y - 15);

    int childXOffset = xOffset / 2;
    int childY = y + 80;

    for (SyntaxTreeNode *child : node->children)
    {
        int childX = x - xOffset + (xOffset / (node->children.size() + 1));
        scene->addLine(x, y, childX, childY);
        drawTree(child, childX, childY, childXOffset);
        xOffset -= childXOffset;
    }
}
