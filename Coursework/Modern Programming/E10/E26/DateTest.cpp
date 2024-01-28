// class date, IO-operators
// author:  Holger Arndt
// version: 01.12.2020

#include <iomanip>
#include <iostream>
#include "Date.hpp"

using namespace std;
using namespace mydate;

int main()
{
  Date d;

  cout << "Enter a date: ";
  cin >> d;
  cout << d << endl;
}
