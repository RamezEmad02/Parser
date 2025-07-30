#ifndef TREEWINDOW_H
#define TREEWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "TinyParser.h"

class TreeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWindow(QWidget *parent = nullptr);
    void setSyntaxTree(SyntaxTreeNode *root);

private:
    QGraphicsScene *scene;
    void drawTree(SyntaxTreeNode *node, int x, int y, int xOffset);

};

#endif // TREEWINDOW_H
