#include <iostream>

using namespace std;

int main(){

    int degree_pol, k;

    std::cout << "Please enter the degree of the polynomial (0 <= n <= 10): ";
    cin >> degree_pol; 

    int actual_size = degree_pol+1;
    int *co_arr = new int[actual_size];

    int i = degree_pol;
    while (i>=0){
        std::cout << "\nCoefficent a[" << i << "]: ";
        cin >> co_arr[i--];
    }

    std::cout << "\nWhich derivative should be computed? ";
    cin >> k; 

    for (int c = degree_pol; c >=0; c--){
        int x_p = c;
        for (int j = k; j > 0; j--){
            co_arr[c] *= x_p--;
        }
    }

    std::cout << "\np^" << k <<"(x) = ";
    for  (int c = degree_pol; c >=0; c--){
        if (co_arr[c] != 0) {
            std::cout << "\n" << co_arr[c] << ".x^" << c-k;
        }
        if (c-k<=0){
            break;
        }
    }
}
