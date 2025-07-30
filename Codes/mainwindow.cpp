#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create the main widget and set it as central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a main horizontal layout to organize the two editors side by side
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left side: Extract Tokens editor and its buttons
    QVBoxLayout *extractTokensLayout = new QVBoxLayout();
    extractTokensTextEdit = new QTextEdit(this);
    extractTokensLayout->addWidget(new QLabel("Extract Tokens", this));
    extractTokensLayout->addWidget(extractTokensTextEdit);

    browseExtractButton = new QPushButton("Browse", this);
    extractTokensLayout->addWidget(browseExtractButton);
    connect(browseExtractButton, &QPushButton::clicked, this, &MainWindow::browseExtractTokensFile);

    saveExtractButton = new QPushButton("Extract Tokens and save", this);
    extractTokensLayout->addWidget(saveExtractButton);
    connect(saveExtractButton, &QPushButton::clicked, this, &MainWindow::saveExtractTokensFile);

    // Right side: Parse Tree editor and its buttons
    QVBoxLayout *parseTreeLayout = new QVBoxLayout();
    parseTreeTextEdit = new QTextEdit(this);
    parseTreeLayout->addWidget(new QLabel("Parse Tree", this));
    parseTreeLayout->addWidget(parseTreeTextEdit);

    browseParseButton = new QPushButton("Browse", this);
    parseTreeLayout->addWidget(browseParseButton);
    connect(browseParseButton, &QPushButton::clicked, this, &MainWindow::browseParseTreeFile);

    generateParseButton = new QPushButton("Generate Parse Tree", this);
    parseTreeLayout->addWidget(generateParseButton);
    connect(generateParseButton, &QPushButton::clicked, this, &MainWindow::generateParseTree);

    // Add the two vertical layouts to the main horizontal layout
    mainLayout->addLayout(extractTokensLayout);
    mainLayout->addLayout(parseTreeLayout);

    // Set the window title and initial size
    setWindowTitle("Token Extractor and Parse Tree Viewer");
    resize(1200, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::browseExtractTokensFile()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File for Extract Tokens"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            extractTokensTextEdit->setPlainText(in.readAll());
            file.close();
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open the file for Extract Tokens"));
        }
    }
}

void MainWindow::saveExtractTokensFile()
{
    // Get the text from the Extract Tokens editor
    QString tokensText = extractTokensTextEdit->toPlainText();

    if (tokensText.isEmpty()) {
        QMessageBox::information(this, tr("No Input"), tr("The Extract Tokens editor is empty."));
        return;
    }

    std::string codeSnippet = tokensText.toStdString();
    std::string x;
    try {
        // Process the text to get the specific vector (assuming you have this function)
        x = scan(codeSnippet);

        // Convert the result to QString
        QString outputText = QString::fromStdString(x);

        // Open a file dialog to save the result
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Extract Tokens File"), "", tr("Text Files (.txt);;All Files ()"));

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << outputText;  // Write the converted string to the file
                file.close();

                lastExtractTokensOutput = fileName; // Save the file path for later use

                // Optionally, load the saved file into the Parse Tree editor
                QFile savedFile(fileName);
                if (savedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&savedFile);
                    parseTreeTextEdit->setPlainText(in.readAll());  // Load into the Parse Tree editor
                    savedFile.close();
                }
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
            }
        }
    } catch (const std::exception& e) {
        // Catch other standard exceptions
        QMessageBox::information(this, tr("Error"), tr("An error occurred:\n%1").arg(e.what()));
        // Convert the result to QString
        QString outputText = QString::fromStdString(codeSnippet);

        // Open a file dialog to save the result
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Extract Tokens File"), "", tr("Text Files (.txt);;All Files ()"));

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << outputText;  // Write the converted string to the file
                file.close();

                lastExtractTokensOutput = fileName; // Save the file path for later use

                // Optionally, load the saved file into the Parse Tree editor
                QFile savedFile(fileName);
                if (savedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&savedFile);
                    parseTreeTextEdit->setPlainText(in.readAll());  // Load into the Parse Tree editor
                    savedFile.close();
                }
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
            }
        }


    } catch (...) {
        // Catch any other unknown exceptions
        QMessageBox::critical(this, tr("Error"), tr("An unknown error occurred."));
    }
}




void MainWindow::browseParseTreeFile()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File for Parse Tree"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            parseTreeTextEdit->setPlainText(in.readAll());
            file.close();
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open the file for Parse Tree"));
        }
    }
}

void MainWindow::generateParseTree()
{
    try{
        QString inputText = parseTreeTextEdit->toPlainText();

        if (inputText.isEmpty())
        {
            QMessageBox::information(this, tr("Generate Parse Tree"), tr("No input provided for Parse Tree generation."));
            return;
        }

    // Call the tree function with inputText
        int result = tree(inputText);
        if (result == 0) // Assume 0 means success
        {
            QMessageBox::information(this, tr("Parse Tree"), tr("Parse Tree generated successfully."));
        }
        else
        {
            QMessageBox::warning(this, tr("Parse Tree"), tr("Failed to generate Parse Tree."));
        }
    }
    catch(const std::exception& e){
        QMessageBox::information(this, tr("Error"), tr("An error occurred:\n%1").arg(e.what()));
    }
}
