#include <iostream>
#include <string>
#include <vector>
#include <regex>    //regular expression used for pattern matching in strings.
#include <fstream>  //used for reading from and writing to files

using namespace std;

// Structure to represent a token
struct Token {
    string value;
    string type;
};

// Function to scan the TINY language code and tokenize it
void scanTINYCode(const string& code, vector<Token>& tokens) {

        // Defines a vector of pair objects, where each pair consists of a token type and a regular expression pattern.
        vector<pair<string, string>> tokenPatterns = {
            {"SEMICOLON", ";"},
            {"IF", "\\bif\\b"},
            {"THEN", "\\bthen\\b"},
            {"ELSE","\\belse\\b"},
            {"END", "\\bend\\b"},
            {"REPEAT", "\\brepeat\\b"},
            {"UNTIL", "\\buntil\\b"},
            {"ASSIGN", ":="},
            {"READ", "\\bread\\b"},
            {"WRITE", "\\bwrite\\b"},
            {"LESSTHAN", "<"},
            {"EQUAL", "="},
            {"PLUS", "\\+"},
            {"MINUS", "-"},
            {"MULT", "\\*"},
            {"DIV", "/"},
            {"OPENBRACKET", "\\("},
            {"CLOSEDBRACKET", "\\)"},
            {"NUMBER", "\\d+"},
            {"IDENTIFIER", "[a-zA-Z]+"}
        };

    size_t position = 0;
    int Line_Number = 1;

    // Loop to scan the code character by character
    while (position < code.length()) {
        bool matched = false;

        // Try matching each token pattern
        for (const auto& pattern : tokenPatterns) {
            regex regexPattern(pattern.second);
            smatch match;

            if (regex_search(code.begin() + position, code.end(), match, regexPattern)
                && match.position() == 0) {
                tokens.push_back({ match.str(), pattern.first });
                position += match.length();
                matched = true;
                break;
            }
        }

        char temp = *(code.begin() + position);
        // If no pattern matches
        if (!matched) {
            if (temp == ' ' || temp == '\n' || temp == '\t') {
                if (temp == '\n') {
                    Line_Number++;
                }
                position++;
            }
            else if (temp == '{') {
                int linesInsideComment = 0;
                while (temp != '}') {
                    if (position >= code.length() - 1) {
                        throw runtime_error("Error: comment unclosed, there is unclosed \"\{\" in line " + to_string(Line_Number));
                    }
                    else {
                        if (temp == '\n') {
                            linesInsideComment++;
                        }
                        position++;
                        temp = *(code.begin() + position);
                    }
                }
                Line_Number += linesInsideComment;
                position++;
            }
            else {
                throw runtime_error("Error: illegal character \"" + string(1, temp) + "\" in line " + to_string(Line_Number));
            }
        }
    }
}

string scan(string& codeSnippet){
    string out="";


    // Read the entire content of the file into a string


    // Scan the code and get the tokens and check if any error occurs
    vector<Token> tokens;
    try {
        scanTINYCode(codeSnippet, tokens);

        // // Output the tokens on the console
        // for (const auto& token : tokens) {
        //     cout << token.value << " , " << token.type << endl;
        // }
        for (const auto& token : tokens) {
            out +=  token.value + "," + token.type + '\n';
        }
        return out;

    }
    catch (const exception& e) {


        // Output the tokens in a file
        for (const auto& token : tokens) {
            out +=  token.value + "," + token.type + '\n';
        }

        // Print the Error message thrown

        codeSnippet = out;
        cout<< codeSnippet;
        throw runtime_error (e.what());
    }
    return out;

}
