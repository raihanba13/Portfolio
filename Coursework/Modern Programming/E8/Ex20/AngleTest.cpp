#include <iostream>
#include <iomanip>
#include "Angle.hpp"

using namespace std; 

int main(){
    Angle a1;

    std::cout << "Enter a Angle (example: 2d30m25s): ";
    cin >> a1;
    cout << a1 << endl;

    cout << "Degrees as Fraction: " << a1.GetAsDegrees() << endl;

    Angle a2;

    std::cout << "Enter another Angle (example: 2d30m25s): ";
    cin >> a2;
    cout << a2 << endl;

    cout << "Sum of two angle: " << a1+a2 << endl;

    // test of normalize
    cout << "Input an angle to normalize (example: 2d30m25s): " ;
    int dd, mm;
    double ss;
    cin >> dd;
    cin.ignore(1, 'd');
    cin >> mm;
    cin.ignore(1, 'm');
    cin >> ss;
    cin.ignore(1, 's');

    Angle a3;
    a3.normalize(dd, mm, ss);
    cout << "Angle after nomalized: " << a3 << endl;

    cout << "Angle in Radian: " << a3.toRadian() << endl;

    return 0;
}