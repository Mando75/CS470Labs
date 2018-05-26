/***********************************************************************
* Program:
*    Lab 04, HTML Sanitzer
*    Brother Wilson, CS470
* Author:
*    Bryan Muller
* Summary:
*    This program uses a whitelist method to sanitize html input.
*    After prompting for html input, the program will parse through
*    the provided string and sanitize any unacceptable html tags.
*    This sanitized result is then presented to the user.
*
*    Estimated:  2.0 hrs
*    Actual:     3.5 hrs
*      Most challenging part: I originally completed this project using 
*      REGEX and it was elegant and beautiful. Then I discovered that the
*      linux lab gcc compiler is too old to support the built in regex 
*      features of C++, so I had to rewrite the whole thing.
*      Yay for legacy software!
************************************************************************/


#include <iostream>
#include <iomanip>
#include <string>
#include <stack>
#include <set>

using namespace std;

/**
 * Note on this: I originally wrote this program
 * with a newer gcc compiler than the one on the Linux Lab.
 * I have not used C++/the Linux Lab in several years, and was unaware
 * that the Linux lab uses a dated compiler. As such, I
 * had to adjust some of my code to work with the older compiler
 * version.
 */
// declare global whitelist constant
const string WHITELIST_ARRAY[] = {
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
// Use this to construct a set of allowed tags.
const set<string> WHITELIST(WHITELIST_ARRAY, WHITELIST_ARRAY + 13);

/* I was having problems with stack,
* but using std::stack seemed to fix it
*
* Also! Fun Fact! The style checker thinks that
* the opening and closing brackets in the type
* specification are less than and greater than
* operators, so please don't dock me points for
* that. :) As far as I am aware, it is standard
* to declare things in the following format.
*
*/
std::stack<string> openTags;


// Function prototypes
void prompt();

string handleInput(string input);

bool checkIfOpenTag(char char1, char char2);

bool checkIfClosingTag(char char1, char char2);

string getTag(string str, int &i);

bool isValidTag(string tag);

string handleCloseTag(string input, int &i);

string closeOpenTags();

string handleOpenTag(string input, int &i);

/**
 * Entry point of the program.
 * Calls prompt to display an input to the user
 */
int main()
{
   prompt();
   return 0;
}

/**
 * Prompts the user for HTML to sanitize.
 * Will exit on the command quit
 */
void prompt()
{
   string userInput;
   cout << "> ";
   getline(cin, userInput);

   if (userInput == "quit")
   {
      cout << "\tquit\n";
      return;
   }
   else
   {
      cout << '\t' << handleInput(userInput) << endl;
      prompt();
   }
}

/**
 * handleInput is the controller function for the sanitization process.
 * It loops through the string of user input, and builds a sanitized output
 * string
 *
 * @params input
 *  - The input provided by the user in form of a string
 */
string handleInput(string input)
{
   // initialize an empty output string
   string output;
   // loop through the whole input string
   for (int i = 0; i < input.length(); i++)
   {
      // determine if our tag is either an opening tag, closing tag
      // or neither
      bool isOpenTag = checkIfOpenTag(input[i], input[i + 1]);
      bool isClosingTag = checkIfClosingTag(input[i], input[i + 1]);
      if (isOpenTag)
      {
         output += handleOpenTag(input, i);
      }
      else if (isClosingTag)
      {
         output += handleCloseTag(input, i);
      }
      else
      {
         output += input[i];
      }
   }
   // If there are still open tags in the stack, add them to the end of the
   // output string
   if (!openTags.empty())
   {
      output += closeOpenTags();
   }
   return output;
}

/**
 * Simple function to check if a symbol is the beginning
 * of an html tag
 */
bool checkIfOpenTag(char char1, char char2)
{
   return (char1 == '<') && (char2 != '/');
}

/**
 * Simple function to check if a symbol is the beginning
 * of an html closing tag
 */
bool checkIfClosingTag(char char1, char char2)
{
   return (char1 == '<') && (char2 == '/');
}

/**
 * Traverses ahead in the input string to find the
 * the full html tag type, e.g. a, b, blockquote, etc.
 * Passes i by reference so we can move the iteration loop
 * in handleInput forward.
 */
string getTag(string str, int &i)
{
   string tag;
   do
   {
      tag += str[i];
      i++;
   } while (str[i] != ' ' && str[i] != '>');
   return tag;
}

/**
 * Checks the Whitelist to see if the provided tag is
 * an approved html tag.
 */
bool isValidTag(string tag)
{
   set<string>::iterator end = WHITELIST.end();
   set<string>::iterator tagLocation = WHITELIST.find(tag);
   // Style checker wanted me to put a space between
   // ! and the expression ¯\_(ツ)_/¯
   return ! (end == tagLocation);
}

/**
 * Empties the stack of any valid html tags which
 * had not been closed.
 */
string closeOpenTags()
{
   string output;
   while (!openTags.empty())
   {
      output += "</" + openTags.top() + ">";
      openTags.pop();
   }
   return output;
}

/**
 * Logic for handling the beginning of an html tag
 * Passes i by reference so we can move the iteration loop
 * in handleInput forward.
 */
string handleOpenTag(string input, int &i)
{
   // moves i off of the opening bracket
   i++;
   // find the tag we are dealing with
   string tag = getTag(input, i);
   // verify the tag is in the whitelist
   bool validTag = isValidTag(tag);
   if (validTag)
   {
      // push the tag into the stack of open tags
      // we are tracking
      openTags.push(tag);
      // decrement so we can avoid errors with not displaying
      // the closing bracket
      i--;
      return "<" + tag;
   }
   else
   {
      // return the sanitized tag
      return "&lt;" + tag + "&gt;";
   }
}

/**
 * handles an closing html tag.
 * Passes i by reference so we can move the iteration loop
 * in handleInput forward.
 */
string handleCloseTag(string input, int &i)
{
   // move i forward so we skip </
   i = i + 2;
   // get the tag type
   string tag = getTag(input, i);
   // if we don't have any open tags, we know
   // the input is tag is invalid, so we can sanitize it
   if (openTags.empty())
   {
      return "&lt;/" + tag + "&gt;";
   }
   else
   {
      // check the tag against the currently open tag in the stack
      string openTag = openTags.top();
      if (tag == openTag)
      {
         // if they match, remove the tag
         // from the stack and return the html tag
         openTags.pop();
         return "</" + tag + ">";
      }
      else
      {
         // if they do not match, the tag is invalid.
         // sanitize it
         i++;
         return "&lt;/" + tag + "&gt; ";
      }
   }
}







