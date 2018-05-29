/***********************************************************************
* Program:
*    Lab 06, Vulnerabilities and Exploits
*    Brother Wilson,, CS470
* Author:
*    Bryan Muller
* Summary: 
*    This program is designed to demonstrate memory injection vulnerabilities.
*
************************************************************************/

#include <iostream>
#include <cassert>
#include <string>
using namespace std;

void stackExploit();
void heapExploit();
void arrayExploit();
void intExploit();
void arcExploit();
void subterfugeExploit();
void vtableExploit();
void ansiExploit();
void stackSmasher();
/******************************************************
 * MAIN
 * This function take you to the various exploits
 *****************************************************/
int main()
{
   // prompt
   cout << "  1. Stack smashing\n"
        << "  2. Heap spraying\n"
        << "  3. Array index\n"
        << "  4. Integer overflow\n"
        << "  5. ARC injection\n"
        << "  6. Pointer subterfuge\n"
        << "  7. VTable spraying\n"
        << "  8. ANSI-Unicode conversion\n"
        << "> ";
   int select;
   cin >> select;

   switch (select)
   {
      case 1:
         stackExploit();
         break;
      case 2:
         heapExploit();
         break;
      case 3:
         arrayExploit();
         break;
      case 4:
         intExploit();
         break;
      case 5:
         arcExploit();
         break;
      case 6:
         subterfugeExploit();
         break;
      case 7:
         vtableExploit();
         break;
      case 8:
         ansiExploit();
         break;
      default:
         cout << "Unknown option " << select << endl;
   }

   return 0;
}

/**************************************************************
 **************************************************************
 *******************    SUBTERFUGE    *************************
 **************************************************************
 **************************************************************/

/****************************************
 * Pointer Subterfuge Vulnerability
 * 1. There must be a pointer used in the code.
 * 2. There must be a way to overwrite the pointer
 * 3. After the pointer is dereferenced, the pointer is dereferenced
 ****************************************/
void subterfugeVulnerability(long * array, int size)
{
   long int buffer[2] = {0, 1};
   const char * message = "All is safe; nothing bad happened\n";  // 1. Pointer

   for (int i = 0; i < size; i++)  // 2. Overwrite
      buffer[i] = array[i];

   cout << message;                // 3. Dereference
   
}

/****************************************
 * Pointer Subterfuge Exploitation
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the pointer
 * 2. The attacker must be able to provide a new pointer
 *    referring to data altering the normal flow of the program
 ****************************************/
void subterfugeExploit()
{
   // an attacker's array
   long int array[3] =             // 1. Exploit
   {
      0,
      1,
      (long int)"!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n"
      // 2. Provide pointer
   };

   // exploit it
   subterfugeVulnerability(array, 3);   
}

/**************************************************************
 **************************************************************
 ********************    STACK       **************************
 **************************************************************
 **************************************************************/

/***********************************************
 * STACK VULNERABILITY
 * 1. There must be a buffer on the stack
 * 2. the buffer must be reachable from an external input
 * 3. The mechanism to fill the buffer must not check the correct buffersize
 **********************************************/
void stackVulnerability(long int userInput[], long int size)
{
   /**
    * Receives a userInput buffer along with its size
    * Copies that buffer into a buffer of size 1.
    */

   // Create a very small buffer
   long int uncheckedBuffer[1];
   // copy the array
   for (long int i = 0; i < size; i++)
   {
      uncheckedBuffer[i] = userInput[i];
      cout << i << " " << uncheckedBuffer[i] << endl;
   }

}


/*********************************************
 * STACK EXPLOIT
 * 1. The attacker must provide more data into the
 *    outwardly facing buffer than the buffer   int buffer[12] = {0, 1, 2, 3, 4, 5, 6, (long int) &displayMessage};

   stackVulnerability(buffer, sizeof(buffer)/sizeof(buffer[0]));
}is designed to hold
 * 2. The attacker must know where the the stack pointer resides
 *    on the stack. This should be just beyond the end of the buffer
 * 3. The attacker must insert machine language instructions in the buffer.
 *    This may occur before, after, or even around the stack pointer. The
 *    machine language could be already compiled code in the program
 * 4. The attacker must overwrite the stack pointer. The old value,
 *    directing the flow of the program after the function is returned,
 *    must be changed from the calling function to the provided
 *    machine language in step 3.
 *********************************************/
void stackExploit()
{
   // create a buffer longer than it should be.
   // Throw a function pointer on the end so we can
   // have some fun.
   long int buffer[10] = { 0, 1, 2, 3, 4, (long int) &stackSmasher};
   int size = sizeof(buffer)/sizeof(buffer[0]);

   cout << "Starting exploit" << endl;
   stackVulnerability(buffer, size);
   cout << "Exploit completed";
}


/**
 * A function to display a message when we've smashed the stack
 */
void stackSmasher()
{
   cout << "Tsk Tsk. You're fired" << endl;
}

/**************************************************************
 **************************************************************
 ********************      HEAP      **************************
 **************************************************************
 **************************************************************/


/*************************************
 * HEAP VULNERABILITY
 * 1. There must be two adjacent heap buffers.
 * 2. The first buffer must be reachable through external input.
 * 3. The mechanism to fill the buffer from the external input must
 *    not correctly check for the buffer size.
 * 4. The second buffer must be released before the first.
 *************************************/
void heapVulnerability(/* feel free to add parameters */)
{
}

/*************************************
 * HEAP EXPLOIT
 * 1. The attacker must provide more data into the outwardly facing
 *    heap buffer than the buffer is designed to hold
 * 2. The attacker must know the layout of the Memory Control Block (MCB)
 *    (essentially a linked list) residing just after the buffer
 * 3. The attacker must provide a new MCB containing both the location
 *    of the memory overwrite and the new data to be overwritten
 ***********************************/
void heapExploit()
{
}

/**************************************************************
 **************************************************************
 ********************     ARRAY      **************************
 **************************************************************
 **************************************************************/

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ************************************/
void arrayVulnerability(/* feel free to add parameters */)  
{
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
}


/**************************************************************
 **************************************************************
 *******************    INTEGER      **************************
 **************************************************************
 **************************************************************/

/********************************************
 * INTEGER VULNERABILITY
 * 1. There must be a security check represented by an expression.
 * 2. The expression must have the potential for overflow.
 * 3. At least one of the numbers used to compute the sentinel
 *    must be reachable through external input.
 *********************************************/
void intVulnerability(/* feel free to add parameters */)
{
}

/**********************************************
 * INTEGER EXPLOIT
 * 1. Provide input, either a buffer size or a single value,
 *    that is directly or indirectly used in the vulnerable expression
 * 2. The input must exceed the valid bounds of the data-type,
 *    resulting in an overflow or underflow condition
 **********************************************/
void intExploit()
{
}

/**************************************************************
 **************************************************************
 ********************      ARC       **************************
 **************************************************************
 **************************************************************/

/*********************************
 * ARC VULNERABILITY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the function pointer. This typically happens through
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must
 *    be dereferenced
 ********************************/
void arcVulnerability(/* feel free to add parameters */)
{
}

/*********************************
 * ARC EXPLOIT
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the function pointer
 * 2. The attacker must have the address to another function
 *    which is to be used to replace the existing function pointer
  ********************************/
void arcExploit()
{
}

/**************************************************************
 **************************************************************
 ********************     VTABLE     **************************
 **************************************************************
 **************************************************************/

/***********************************
 * VULNERABILITY
 * 1. The vulnerable class must be polymorphic.
 * 2. The class must have a buffer as a member variable.
 * 3. Through some vulnerability, there must be a way for user input
 *    to overwrite parts of the VTable.
 * 4. After a virtual function pointer is overwritten,
 *    the virtual function must be called.
 **********************************/
class Vulnerability
{
};

/************************************
 * VTABLE EXPLOIT
 * 1. Through some vulnerability, the VTable pointer
 *     or a function pointer within the VTable must be overwritten
 * 2. The attacker must have the address to another VTable pointer
 *     or a function pointer
 ***********************************/
void vtableExploit()
{
}


/**************************************************************
 **************************************************************
 ******************    ANSI-Unicode    ************************
 **************************************************************
 **************************************************************/

/*********************************************************
 * ANSI - UNICODE VULNERABILITY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer
 *    rather than the number of elements in the buffer.
 ********************************************************/
void ansiVulnerability(/* feel free to add parameters */)
{
}

/***********************************************
 * ASCI - UNICODE EXPLOIT
 * 1. The attacker must provide more than half as much data
 *    into the outwardly facing buffer as it is designed to hold
 **********************************************/
void ansiExploit()
{
}
