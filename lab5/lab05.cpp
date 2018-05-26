/***********************************************************************
* Program:
*    Lab 05, The stack
*    Brother Wilson, CS470
* Author:
*    Bryan Muller
* Summary: 
*    This program is designed to illustrate weaknesses in the stack.
*    It will display addresses from the stack, heap and code segments
*
************************************************************************/

#include <stdlib.h>    // for malloc()
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;


void partOne(int); // Find the address of the stack, heap, and code segments
void partTwo(char *, long);    // Manipulate the stack to find a weakness
void partThree();  // Demonstrate a variety of vulnerabilities

/******************************************************
 * MAIN
 * This function will only take you to one of the three parts
 * of Assignment 05
 *****************************************************/
int main()
{
   char text[] = "%%main%%";
   long number = 1234567;
   &number;

   // prompt
   cout << "Please select an option:\n";
   cout << "  1.  Find the address\n";
   cout << "  2.  Display the contents of the stack\n";
   cout << "  3.  Manipulate the stack\n";
   cout << "> ";

   int selection;
   cin >> selection;

   switch (selection)
   {
      case 1:
         partOne(1);
         break;
      case 2:
         partTwo(text, number + 1111111);
         break;
      case 3:
         partThree();
         break;
      default:
         cout << "Unknown option\n";
         return 1;
   }

   return 0;
}

/**************************************************************
 **************************************************************
 **************************** PART 1 **************************
 **************************************************************
 **************************************************************/


/**********************************************
 * Part 1
 *
 * Find the address of the stack, heap, and code.
 * Note that it will be in a different location with
 * every execution
 **********************************************/
void partOne(int byValueParameter)
{
   void *pStack = NULL;
   void *pHeap = NULL;
   void *pCode = NULL;

   ////////////////////////////////////////////////
   // put your magic code between here...

   // this will reassign pStack's value to a stack address
   pStack = &pStack;
   // initialize some space on the heap by using 'new'
   pHeap = new char();
   // create a function pointer
   pCode = (void *) partOne;
   // ... and here
   ////////////////////////////////////////////////

   // display the results
   cout << "Stack: " << pStack << endl;
   cout << "Heap:  " << pHeap << endl;
   cout << "Code:  " << pCode << endl;

   return;
}

/**************************************************************
 **************************************************************
 **************************** PART 2 **************************
 **************************************************************
 **************************************************************/

/************************************************
 * CONVERT TO STRING
 * Convert the data from p into a human-readable string
 * by removing all the unprintable characters and replacing
 * them with a dot
 ***********************************************/
string displayCharArray(const char *p)
{
   string output;
   for (int i = 0; i < 8; i++)
      output += string(" ") + (p[i] >= ' ' && p[i] <= 'z' ? p[i] : '.');
   return output;
}

/*************************************************
 * PART 2
 *
 * Display the contents of the stack. This will be from
 * slot -4 to slot 20.
 *************************************************/
void partTwo(char *text, long number)
{
   // start your display of the stack from this point
   long bow = number + 1111111;

   // header for our table. Use these setw() offsets in your table
   cout << "[" << setw(2) << 'i' << ']'
        << setw(15) << "address"
        << setw(20) << "hexadecimal"
        << setw(20) << "decimal"
        << setw(18) << "characters"
        << endl;
   cout << "----+"
        << "---------------+"
        << "-------------------+"
        << "-------------------+"
        << "-----------------+\n";
   for (int i = -4; i <= 20; i++)
   {

      ////////////////////////////////////////////////
      // put your magic code between here...

      // formatting
      if (i <= 9 && i >= 0)
      {
         cout << "[ " << i << "]";
      }
      else
      {
         cout << "[" << i << "]";
      }
      // Create a pointer to the value we will be displaying
      char **stackVal = (&(text) + i);
      // output the stack address
      cout << setw(15) << &stackVal;
      // output the hex value of the address
      cout << setw(20) << hex << (long) *&stackVal;
      // output the decimal value of the address
      cout << dec << setw(20) << (long) *&stackVal;
      // output the character value of the address using the provided function
      cout << setw(20) << displayCharArray((char *) stackVal) << endl;
      // ... and here
      ////////////////////////////////////////////////
   }
}

/**************************************************************
 **************************************************************
 **************************** PART 3 **************************
 **************************************************************
 **************************************************************/

void fail(char grade);

void pass(char grade);

/**********************************************************************
 * All your code needs to exist in this function below the comment
 *
 * This function will manipulate the stack to change function calls
 * and variable values.
 *********************************************************************/
void readStack()
{
   int number;
   char text[16];

   ////////////////////////////////////////////////
   // put your magic code between here...

   // Loop through the stack and find the pointer to the fail function
   // fail is our entry point into the vulnerability
   long *searchPtr;

   for (searchPtr = (long *) &searchPtr; *searchPtr != (long) fail; searchPtr++);

   // once we have found the pointer to the fail function, we can replace it
   // with a pointer to the pass function
   *searchPtr = (long) pass;

   /**
    * From examining the fail function, we know where it's local
    * variables will be located in the stack. They will be in the
    * following order:
    *
    * letterGrade
    * number
    * text
    *
    * We can move through the stack with our search pointer and
    * use it to change the values of these variables.
    */

   // Changing letterGrade
   searchPtr--;
   /**
    * Because letterGrade is a character,
    * we need to cast searchPtr to a char *.
    * Also, characters don't fill the whole byte
    * so we only change index 7 of the memory location
    */
   ((char *) searchPtr)[7] = 'A';


   // Extract the number using the same method
   // as with the letterGrade
   number = *(int *) searchPtr;

   // Text is 16 bytes long, so we need to move back 2 memory addresses

   searchPtr -= 2;
   char *textPtr = text;
   char *searchCharPtr = (char *) searchPtr;

   // transfer each value in the character array from the
   // the value in text from partThree to text in readStack
   // using the pointers
   for (textPtr; *(textPtr++) = *(searchCharPtr++););

   // ... and here
   ////////////////////////////////////////////////

   // output the goods
   cout << "\tnumber: "
        << number
        << endl;
   cout << "\ttext:   "
        << text
        << endl;

   return;
}


/*******************************************************************
 * Part 3
 *
 * This function contains the data that you are trying to manipulate.
 * Please don't modify partTwo() in any way
 ********************************************************************/
void partThree()
{
   int number;   // first local variable that you need to read
   char text[16]; // second

   char letterGrade;
   void (*pointerFunction)(char);

   // get the values
   cout << "number: ";
   cin >> number;
   cout << "text:   ";
   cin >> text;


   // Call the function and display the results.
   pointerFunction = fail;
   letterGrade = 'B';
   readStack();            // vulnerability is here 
   (*pointerFunction)(letterGrade);

   return;
}


/**********************************************************************
 * The failure case. Don't touch this function
 *********************************************************************/
void fail(char grade)
{
   cout << "\tI am sorry, but you got a 'F'\n";
}

/**********************************************************************
 * I think you would really rather call this function.  This
 * is read-only also
 *********************************************************************/
void pass(char grade)
{
   cout << "\tGood job, you got a(n) '" << grade << "'\n";
}


