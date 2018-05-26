/*************************************************************************
 * CS 416 Week 04: White Box Testing
 *   This program is not designed to model good coding style or
 *   good coding practices. It is meant only as an example for
 *   White Box Testing
 *************************************************************************/

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

bool getInput (string &input)
{
   bool dataToProcess = true;

   cout << "Input: ";
   cin >> input;
   for (int i = 0; i < input.length (); ++i)
      input.at (i) = toupper (input.at (i));
   if (input == "QUIT")
      dataToProcess = false;
   return dataToProcess;
}

int getDecimal (string input)
{
   int rToI[128] = {0};
   int decimal[64];
   int total = 0;

   rToI['I'] = 1;
   rToI['V'] = 5;
   rToI['X'] = 10;
   rToI['L'] = 50;
   rToI['C'] = 100;
   rToI['D'] = 500;
   rToI['M'] = 1000;
   for (int i = 0; i < input.length (); ++i)
      decimal[i] = rToI[input.at (i)];
   for (int i = 0; i < input.length () - 1; ++i)
      if (decimal[i] < decimal[i + 1])
         decimal[i] *= -1;
   for (int i = 0; i < input.length (); ++i)
      total += decimal[i];
   return total;
}

string getRoman (int decimal)
{
   string output = "";
   int divisor;
   string roman[4][10] = {{"", "I", "II", "III", "IV",
                           "V", "VI", "VII", "VIII", "IX"},
                          {"", "X", "XX", "XXX", "XL",
                           "L", "LX", "LXX", "LXXX", "XC"},
                          {"", "C", "CC", "CCC", "CD",
                           "D", "DC", "DCC", "DCCC", "CM"},
                          {"", "M", "MM", "MMM", "", "", "", "", "", ""}};

   for (int i = 3; i >= 0; --i)
   {
      divisor = static_cast<int> (pow (10.0, i));
      output += roman[i][decimal / divisor];
      decimal %= divisor;
   }
   return output;
}

bool valid (string input)
{
   string roman;
   int decimal;

   decimal = getDecimal (input);
   roman = getRoman (decimal);
   return input == roman;
}

int main (void)
{
   string input;

   cout << "Enter a Roman numeral or quit\n";
   while (getInput (input))
   {
      if (!valid (input))
         cout << "in";
      cout << "valid\n";
   }
   return 0;
}
