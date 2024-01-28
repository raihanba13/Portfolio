#include <iostream>
#include <math.h>

using namespace std;

float quadratic_eq(double a, double b, double c, double& r1, double& r2){
    if ((b*b-4*a*c) > 0) {
        r1 = (-b + sqrt(b*b-4*a*c))/(2*a);
        r2 = (-b - sqrt(b*b-4*a*c))/(2*a);
        return true;
    } else if ((b*b-4*a*c) == 0){
        r1 = -b / (2*a);
        r2 = r1;
        return true;
    }else{
        return false;
    }
}

int main(){

    double a, b, c, r1, r2;
    bool flag;

    std::cout << "a,b, c: " ;
    cin >> a; 
    cin >> b; 
    cin >> c; 

    flag = quadratic_eq(a, b, c, r1, r2);

    if (flag && r1 != r2){
        std::cout << "r1= " << r1 << "\n";
        std::cout << "r2= " << r2;
    }else if(flag && r1==r2){
        std::cout << "r1=r2=" << r1;
    }else {
        std::cout << "No real solutions!";
    }
}