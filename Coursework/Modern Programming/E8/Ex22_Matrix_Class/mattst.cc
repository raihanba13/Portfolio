// class matrix, test program
// author:  Holger Arndt
// version: 17.12.2019

#include <iostream>

#include "matrix.h"

using namespace std;

double norm1(const Matrix& mat){
    int rows = mat.numrows();
    int cols = mat.numcols();

    double largest_norm = 0.0;

    for (int j=0; j< cols; ++j){
        double temp_norm = 0.0;
        for (int i=0; i<rows; ++i){
            temp_norm += (double) mat(i, j);
        }
        if(largest_norm < temp_norm){
            largest_norm = temp_norm;
        }
    }

    return largest_norm;
}

int main() {
    Matrix m1;
    cout << "m1: " << m1;

    Matrix m2 {
        3,
        2
    };
    cout << "m2: " << m2;

    Matrix m3 {
        3
    };
    cout << "m3: " << m3;

    Matrix m4 {
        3,
        3,
        127
    };
    cout << "m4: " << m4;

    Matrix m5 {
        m4
    };
    cout << "m5: " << m5;

    m5(0, 2) = 123;
    cout << "m5: " << m5;

    cout << "m2.numrows: " << m2.numrows() << endl;
    cout << "m2.numcols: " << m2.numcols() << endl;
    cout << "m2.numelems: " << m2.numelems() << endl;

    m2 = m5;
    cout << "m2: " << m2;

    double m6arr[] {
        1,
        2,
        3,
        4
    };
    Matrix m6 {
        2,
        2,
        m6arr
    };
    cout << "m6: " << m6;

    m5 = 99;
    cout << "m5: " << m5;

    m6.setall(4, 5.0, 6.0, 7.0, 8.0); // NOT setall(4, 5, 6, 7, 8) !!
    cout << "m6: " << m6;

    m3 = 1;
    cout << "m3 + m5: " << m3 + m5;

    cout << "m3 - m5: " << m3 - m5;

    cout << "2 * m5: " << 2 * m5;
    m4 = 2 * m5;
    cout << "m4 = 2 * m5: " << m4;

    cout << "m3 * m2: " << m3 * m2;

    cout << "m3 == m5: " << (m3 == m5 ? "yes" : "no") << endl;
    m3 = m5;
    cout << "m3 == m5: " << (m3 == m5 ? "yes" : "no") << endl;

    #ifdef DEBUG
    // this last line should give an error message in the debug version
    m3(1, 6) = 9;
    #endif

    double test1arr[] {
        1.0,
        2.0,
        3.0,
        4.0,
        5.0,
        6.0
    };

    Matrix test1 {
        2,
        3,
        test1arr
    };

    cout << "test1: " << test1;
    

    double test2arr[] {
        1.5,
        1.0,
        1.0,
        1.0,
        1.5,
        1.5
    };

    Matrix test2 {
        2,
        3,
        test2arr
    };

    cout << "test2: " << test2;

    test1 += test2;
    cout << "test1+=test2: " << test1;

    double norm = norm1(test1);
    cout << "Norm of test1 Matrix: " << norm << endl;

    Matrix transposed_test1; 
    transposed_test1 = test1.transposed();
    cout << "Transposed Test1: " << transposed_test1;
}