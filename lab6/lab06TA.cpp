/***********************************************************************
* Program:
*    Lab 06, Vulnerabilities and Exploits
* Summary: 
*    This program is designed to demonstrate memory injection vulnerabilities.
*
************************************************************************/

#include <iostream>
#include <cassert>
using namespace std;

void stackExploit();
void heapExploit();
void arrayExploit();
void intExploit();
void arcExploit();
void subterfugeExploit();
void vtableExploit();
void ansiExploit();

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
 ********************    STACK       **************************
 **************************************************************
 **************************************************************/

/***********************************************
 * STACK VULNERABILITY
 * 1. There must be a buffer on the stack
 * 2. the buffer must be reachable from an external input
 * 3. The mechanism to fill the buffer must not check the correct buffersize
 **********************************************/
void stackVulnerability(long int uncheckedBuffer[], long int num)
{
   // small vulnerable buffer.  Poor buffer!
   long int i = 0;
   long int vulnerableBuffer[2];                  // 1. Buffer on the stack

   // copy the array
   for (i = 0; i < num; i++)                      // 3. Wrong buffer size
   {
      cout << i << " " << vulnerableBuffer[i] << endl;
      vulnerableBuffer[i] = uncheckedBuffer[i];   // 2. Buffer reachable     
   }
}

/*********************************************
 * HACKED
 * the code that will be called after stackVulnerability()
 * returns instead of the expected return to stackExploit()
 *********************************************/
void hacked()
{
   cout << "This definitely does not look like Kansas!\n";
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
   long int buffer[12] =                         // 1. more data
      { 0, 1, 2, 3, 4, 5, 6, (long int)&hacked };      // 2. stack pointer
                                                 // 3. compiled code

   cout << "address of stackExploit: "
        <<  hex << (void *)&stackExploit << endl;
   cout << "stackExploit() -- begin\n";
   stackVulnerability(buffer,                    // 4. overwrite stack pointer
                      sizeof(buffer)/sizeof(buffer[0]));
   cout << "stackExploit() -- end\n";
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
void heapVulnerability(long *buffer, long size)
{
   // need to have two buffers.
   long int *buffer1 = new long[4];
   long int *buffer2 = new long[4];

   // we must get the order right!
   assert(buffer1 < buffer2);                 // 1. adjacent
   assert(buffer1 + 6 == buffer2);

   // exploit... overwrite the first buffer
   // this is heap spraying....
   for (int i = 0; i < size; i++)             // 3. incorrect bounds checking
      buffer1[i] = 0x00;                      // 2. external input

   // free the second buffer first!
   delete [] buffer2;                         // 4. relase the second first

   // the rest doesn't matter
   delete [] buffer1;   
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
   long int buffer[6] =                      // 1. more data
   //               MCB is here              // 2. know layout of MCB
      { 0, 0, 0, 0, -1, -1};                 // 3. heap spraying

   cout << "heapExploit() -- begin\n";
   heapVulnerability(buffer, sizeof(buffer) / sizeof(buffer[0]));
   cout << "heapExploit() -- end\n";   
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
void arrayVulnerability(int index, char value)  // 2. index through external
{
   char array[4] = {};                          // 1. an array

   // here is the exploit
   assert(array[index] == 'n');
   array[index] = value;                        // 3. no bounds checking
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
   // just some random program logic
   char authenticated = 'n';
   cout << "We are not currently authenticated\n";
   
   // here is the exploit
   arrayVulnerability(47,                // 1. expected range is 0 ... 3
                      'y');              // 2. provide input

      // now for the payload
   if (authenticated == 'y')             // 3. alter program state
      cout << "\tWe are authenticated; you are a trusted user!\n";
   else
      cout << "\tYou are not authenticated. Nothing dangerous will happen\n";
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
void intVulnerability(bool buffer[],          
                      unsigned char max,    // 2. potential for overflow
                      int index,            // 3. at least one of the numbers
                      bool value)           
{
   // do a "safe" check to make sure we are not outside our bounds
   if (index >= 0 && (unsigned char)index <= max)      // 1. security check
      buffer[index] = value;
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
   bool buffer[256];
   bool authenticated = false;

   intVulnerability(buffer,          
                    256 - 1,              
                    271,     // 1,2 value is 256 which is zero when cast
                    true);   //     to an unsigned char.

   if (authenticated)
      cout << "You have access to everything\n";
   else
      cout << "Access is denied\n";
}

/**************************************************************
 **************************************************************
 ********************      ARC       **************************
 **************************************************************
 **************************************************************/

/**********************************
 * SAFE
 * A simple safe function
 **********************************/
void safe()
{
   cout << "All is well; your secret is safe with me!\n";
}

/**********************************
 * DANGEROUS
 * A simple dangerous function
 **********************************/
void dangerous()
{
   cout << "Your 'secret' password is \"rosebud\".\n";
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
void arcVulnerability(long * array, int size)
{
   long int buffer[2]; 
   void (*pointerFunction)() = safe;       // 1. function pointer

   for (int i = 0; i < size; i++)          // 2. user input overwrites
      buffer[i] = array[i];

   // this should say 'All is well'
   pointerFunction();                      // 3. dereferenced
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
   // an attacker's array
   long int array[3] = {1, 1,
                        (long int )dangerous};  // 2. address to other function
   
   // exloit it
   arcVulnerability(array, 3);                  // 1. exploit vulnerability
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
   long int buffer[2];
   const char * message = "Safe";  // 1. Pointer

   for (int i = 0; i < size; i++)  // 2. Overwrite
      buffer[i] = array[i];

   cout << "Message is: \""
        << message                 // 3. Dereference
        << "\".\n";
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
      1,
      1,
      (long int)"Dangerous"        // 2. Provide pointer
   };

   // exploit it
   subterfugeVulnerability(array, 3);
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
   long numbers[2];                     // 2. buffer as member variable

   /************************************************
    * The vulnerable function
    ***********************************************/
   void setNumber(long value, int index)
   {
      numbers[index] = value;           // 3. overwrite parts of vtable
   }
   virtual void display() const = 0;    // 1. polymorphic function
};

/**************************************
 * SAFE
 **************************************/
class Safe : public Vulnerability
{
public:
   virtual void display() const         // 4. call the virtual function
   {
      cout << "Your numbers are safe! "
           << numbers[0] << ", " << numbers[1] << endl;
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
   Safe s;
   void (*pointerFunction)() = dangerous;   // 2. provide another pointer

   s.setNumber((long)pointerFunction, -2);  // 1. overwrite the pointer
   s.display();
}


/**************************************************************
 **************************************************************
 ******************    ANSI-Unicode    ************************
 **************************************************************
 **************************************************************/

#include <stdlib.h> // for malloc()

/*********************************************************
 * ANSI - UNICODE VULNERABILITY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer
 *    rather than the number of elements in the buffer.
 ********************************************************/
short * ansiVulnerability(const char * source, int size)
{
   short * unicode;                   // 1. basetype of a short is 2
   unicode = (short *)malloc(size);   // 2. malloc requires the number of bytes

   for (int i = 0; i < size; i++)
      unicode[i] = (short)source[i];

   return unicode;
}

/***********************************************
 * ASCI - UNIODE EXPLOIT
 * 1. The attacker must provide more than half as much data
 *    into the outwardly facing buffer as it is designed to hold
 **********************************************/
void ansiExploit()
{
   short * data = ansiVulnerability("12345678901234567890", 20);
   free(data);
}