/***********************************************************************
* Program:
*    Lab 06, Vulnerabilities and Exploits
*    Brother Wilson, CS470
* Author:
*    Bryan Muller
* Summary: 
*    This program is designed to demonstrate memory injection vulnerabilities.
*    Note: StyleChecker is saying that I am missing proper indentation
 *         but in my source file, everything is indented correctly.
 *         I'm not sure what's up with that.
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
void subterfugeVulnerability(long *array, int size)
{
   long int buffer[2] = {0, 1};
   const char *message = "All is safe; nothing bad happened\n";  // 1. Pointer

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
         (long int) "!!!!!!!!!!!!! You've been hacked !!!!!!!!!!!!!\n"
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
 *    outwardly facing buffer than the buffer is designed to hold
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
   long int buffer[10] = {0, 1, 2, 3, 4, (long int) &stackSmasher};
   int size = sizeof(buffer) / sizeof(buffer[0]);

   cout << "Demonstrating exploit by inserting a function address into the "
        << "call stack" << endl;
   cout << "This demonstrates how a dangerous function call could be "
        << "introduced into the stack" << endl;
   cout << "Beginning stack smash" << endl;
   stackVulnerability(buffer, size);
   cout << "Stack smashed" << endl;
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
void heapVulnerability(long int *userInput, int size)
{
   // Create two buffers on the Heap
   long int *vulBuffer1 = new long[2];
   long int *vulBuffer2 = new long[2];

   // overwrite the first buffer
   // without checking bounds
   for (int i = 0; i < size; i++)
   {
      vulBuffer1[i] = userInput[i];
   }

   // remove the second before the first
   delete[] vulBuffer2;
   delete[] vulBuffer1;
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
   // use this buffer to spray the heap
   // with new values
   long int buffer[6] = {-5, -5, -5, -5, -5, -5};

   cout << "We will now employ heap spraying to crash the program" << endl;
   heapVulnerability(buffer, sizeof(buffer) / sizeof(buffer[0]));
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
void arrayVulnerability(int i, int userInput)
{
   // initialize an array
   int vulnerableArray[2] = {1, 2};
   // For debugging
   assert(vulnerableArray[i] == 0);
   // set the ith value of the array to the
   // provided value with no bounds checking
   cout << "Putting in address " << hex << &vulnerableArray[i] << endl;
   vulnerableArray[i] = userInput;
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
   // Create a variable
   int meaningOfLife = 0;
   cout << "We will demonstrate an array index exploit by changing the value"
        << " of meaningOfLife" << endl;
   cout << "Intial value for meaningOfLife: " << meaningOfLife << endl;
   cout << "Meaning of life address: " << hex << &meaningOfLife << endl;

   /**
    * Pass an array index to access and a new value.
    * This exploit should use the unchecked array bounds
    * in the vulnerability to change the value of meaningOfLife
    * to 42.
    *
    * NOTE: I was getting this to work on the linux lab computer,
    * but not on my local machine as the compiler is different.
    * Depending on which machine the program is compiled on, you
    * may need to provide a different index to the vulnerability
    * to get it to change meaningOfLife.
    */
   cout << "Use the vulnerability to change the value of meaningOfLife"
        << endl;
   arrayVulnerability(15, 42);
   cout << "The meaning of life is " << dec << meaningOfLife << endl;
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
void intVulnerability(char exploitBuffer[], unsigned char size,
                      int index, int exploitVal)
{
   if ((unsigned char) index <= size && index >= 0)
   {
      cout << "Putting in address " << hex << &exploitBuffer[index] << endl;
      exploitBuffer[index] = exploitVal;
   }
   else
   {
      cout << dec << (unsigned char) index << endl;
   }
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
   // Create a new buffer
   // Using characters since they have a small size
   char buffer[256];
   // This is the variable we will change using the
   // exploit
   int theMeaningOfLife = 0;
   // for finding the correct address
   cout << "Again we will use an integer overflow exploit to change"
        << " the value of theMeaningOfLife" << endl;
   cout << "Initial value: " << dec << theMeaningOfLife << endl;
   cout << "Meaning of life address: " << hex
        << &theMeaningOfLife << endl;


   /**
    * This is tricky. When the 268 is cast to an
    * unsigned char for the comparison, it overflows
    * resulting in a zero. This allows us to bypass the
    * size check comparison as index is still > 0 when
    * not cast. This allows us to change the value in
    * theMeaningOfLife. Again, depending on which computer
    * you compile and run this on, you will need to change
    * the value of 268.
    */
   cout << "Invoke the exploit" << endl;
   intVulnerability(buffer, 255, 268, 42);

   cout << "The meaning of life is: " << dec << theMeaningOfLife << endl;

}

/**************************************************************
 **************************************************************
 ********************      ARC       **************************
 **************************************************************
 **************************************************************/

/**
 * The function that is supposed to be executed
 * by the ARC Vulnerability function
 */
void theMeaningOfLife()
{
   cout << "The Meaning of Life is 42" << endl;
}

/**
 * The function we will call using
 * the ARC exploit function
 */
void compromisedMeaningOfLife()
{
   cout << "Life is meaningless" << endl;
}

/*********************************
 * ARC VULNERABILITY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the function pointer. This typically happens through
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must
 *    be dereferenced
 ********************************/
void arcVulnerability(long *userInput, int size)
{
   // Create a buffer to copy user input into
   long uncheckedBuffer[2];
   // Here is our function pointer, right
   // after the unchecked buffer in memory
   void (*pntFunc)() = theMeaningOfLife;

   for (int i = 0; i < size; i++)
   {
      // copy all of userInput into the
      // vulnerable buffer without checking size
      uncheckedBuffer[i] = userInput[i];
   }

   // Call our function pointer to 'theMeaningOfLife'
   pntFunc();
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
   // Create a payload with a pointer to a new function
   long payload[3] = {0, 0, (long) compromisedMeaningOfLife};
   cout << "The arcVulnerability function should call the "
        << "meaningOfLife function to display its message"
        << ". \nUsing ARC injection, we will overwrite the "
        << "function pointer and inject a compromised function "
        << "in its place" << endl;
   arcVulnerability(payload, 3);
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
public:
   // the buffer we will overflow
   long buffer[2];

   /* our function that sets the buffer with no bounds
   * check
   */
   void setBuffer(long newVal, int i)
   {
      buffer[i] = newVal;
   }

   /* our virtual function to be
   // implmented in our child class
    */
   virtual void getBuffer() = 0;
};

/**
 * Child class that inherits some dangerous
 * code from Vulnerability
 */
class Dangerous : public Vulnerability
{

public:

   /*
    * implement the getBuffer virtual function
    * to display the contents of buffer
    */
   virtual void getBuffer()
   {
      cout << "Buffer contents " << buffer[0]
           << " " << buffer[1] << endl;
   }
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
   // new class instance
   Dangerous exploit;
   // our function pointer
   void (*pntFun)() = compromisedMeaningOfLife;

   cout << "Demonstrates VTABLE exploit by overwriting"
        << " the pointer to the getBuffer method on the vtable"
        << " and inserting a pointer to our compromised function instead.\n"
        << " Instead of printing out the contents of the buffer, it will"
        << " execute our compromised function" << endl;
   // use the setBuffer to overflow the buffer
   // and overwrite the function pointer on the vtable
   exploit.setBuffer((long) pntFun, -2);
   // call the getBuffer method
   exploit.getBuffer();
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
short *ansiVulnerability(const char *inputText, int size)
{
   // allocate a new array on the heap
   short *toCopy = new short;

   for (int i = 0; i < size; i++)
   {
      // copy the string into the short
      toCopy[i] = (short) inputText[i];
   }

   // return a pointer to the newly allocated
   // unicode value
   return toCopy;
}

/***********************************************
 * ASCI - UNICODE EXPLOIT
 * 1. The attacker must provide more than half as much data
 *    into the outwardly facing buffer as it is designed to hold
 **********************************************/
void ansiExploit()
{
   /**
    * I'm not sure if this is what we were supposed to do,
    * but this will spray the heap and crash the program
    */
   short *value = ansiVulnerability("abcdefghijklmnopqrstuvwxyz", 26);
   delete value;
}
