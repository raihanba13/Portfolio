#include <iostream>
#include <stdlib.h> 
#include <time.h> 

using namespace std;

int main(){

    int n, k;

    std::cout << "n: ";
    cin >> n; 

    int *r = new int[n+1];

    /* seed for rand */
    srand(time(NULL));

    double mean = 0;
    for (int m =0; m < n; m++){
        r[m] = 1 + rand() % 6 ;
        // for debugging purpose
        std::cout << "\nRandom Number " << m << ": " << r[m];
        mean += r[m];
    }

    std::cout << "\nThe avg: " << mean/n;

}