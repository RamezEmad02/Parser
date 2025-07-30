#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QLabel>

int tree(QString text);
std::string scan(std::string& codeSnippet);
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseExtractTokensFile();   // Slot to browse a file for Extract Tokens
    void saveExtractTokensFile();     // Slot to save the Extract Tokens output
    void browseParseTreeFile();       // Slot to browse a file for Parse Tree
    void generateParseTree();         // Slot to generate the Parse Tree

private:
    // Widgets for Extract Tokens
    QTextEdit *extractTokensTextEdit; // Text editor for Extract Tokens
    QPushButton *browseExtractButton; // Button to browse for Extract Tokens
    QPushButton *saveExtractButton;   // Button to save Extract Tokens

    // Widgets for Parse Tree
    QTextEdit *parseTreeTextEdit;    // Text editor for Parse Tree
    QPushButton *browseParseButton; // Button to browse for Parse Tree
    QPushButton *generateParseButton; // Button to generate Parse Tree

    // File location tracking
    QString lastExtractTokensOutput; // File path for the Extract Tokens output
};

#endif // MAINWINDOW_H
