#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>


NewWindow::NewWindow(QWidget *parent,QString text) : QDialog(parent) {
    setWindowTitle("Parse tree");
    setFixedSize(1000, 1000);

    QVBoxLayout *layout = new QVBoxLayout(this);



}
