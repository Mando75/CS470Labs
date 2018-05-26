/***********************************************************************
* Program:
*    Week 04, lab 04       (e.g. Assignment 10, Hello World)
*    Brother Wilson, CS470
* Author:
*    Bryan Muller
* Summary:
*    Enter a brief description of your program here!  Please note that if
*    you do not take the time to fill out this block, YOU WILL LOSE POINTS.
*    Before you begin working, estimate the time you think it will
*    take you to do the assignment and include it in this header block.
*    Before you submit the assignment include the actual time it took.
************************************************************************/


#include <iostream>
#include <iomanip>
#include <string>
#include <stack>

using namespace std;

// declare global whitelist constant
const string WHITELIST[] = {
        "a",
        "abbr",
        "acronym",
        "b",
        "blockquote",
        "cite",
        "code",
        "del",
        "em",
        "i",
        "q",
        "strike",
        "strong"
};


// Function decelerations
void prompt();
string handleInput(string input);
bool checkIfBracket(char character);
string getTag(string str, int& i);
bool checkValidTag(string tag);
string makeWhitelistPattern();
string sanitizeBrackets(string tag);

int main() {
    prompt();
    return 0;
}


void prompt() {
    string userInput = "";
    cout << ">";
    getline(cin, userInput);

    if(userInput == "quit") {
        return; 
    } else {
        cout << handleInput(userInput) << endl;
        prompt();
    }
}


string handleInput(string input) {
    string output = "";
    stack<string> openTags;
    for (int i = 0; i < input.length(); i++) {

        bool isBracket = checkIfBracket(input[i]);
        if(isBracket) {
            string tag = getTag(input, i);
            bool validTag = checkValidTag(tag);
            if(!validTag) {
                cout << "Invalid tag " << tag <<  endl;
                output += sanitizeBrackets(tag);
            } else {
                output += tag;
                openTags.pop();
            }
        } else {
            output += input[i];
        }

    }
    return output;
}

bool checkIfBracket(char character) {
    return character == '<' || character == '>';
}



string getTag(string str, int& i) {
    string tag = "";
    if(str[i] == '<') {
        do {
            tag += str[i];
        } while (str[i++] != '>');
    }
    return tag;
}

bool checkValidTag(string tag) {
    regex pattern("<(?!\\/?(" + makeWhitelistPattern() + ")\\b)[^>]+>");
    return !regex_match(tag, pattern);

}

string makeWhitelistPattern() {
    string output = "";
    for(string str : WHITELIST) {
        output += (str + "|");
    }
    output.pop_back();
    return output;
}

string sanitizeBrackets(string tag) {
    regex left("<");
    regex right(">");
    tag = regex_replace(tag, left,"&lt;" );
    tag = regex_replace(tag, right, "&gt;");
    return tag;
}





