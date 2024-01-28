// class matrix, implementation-file
// author:  Holger Arndt
// version: 01.12.2020

#include <cstdarg>

#include <iomanip>

#include <iostream>

#include "matrix.h"

using namespace std;

// implementation of private methods

// initialize a new (m × n)-matrix
void Matrix::initialize(int m, int n) {
    mm = m;
    nn = n;
    data = new double[mm * nn]; // allocate memory for the elements
    rows = new double * [mm]; // allocate memory for row pointers
    double * p;
    int i;
    for (p = data, i = 0; i < mm; ++i) {
        rows[i] = p; // initialize row pointers
        p += nn;
    }
}

void Matrix::destroy() {
    delete[] data;
    delete[] rows;
}

// set all elements to val
void Matrix::setelems(double val) {
    double * p {
        data
    };
    double * pend {
        data + mm * nn
    };
    for (; p < pend; ++p)
        *
        p = val;
}

// implementation of public methods

// move constructor
Matrix::Matrix(Matrix && mat2) {
    mm = mat2.mm;
    nn = mat2.nn;
    data = mat2.data;
    rows = mat2.rows;
    mat2.mm = mat2.nn = 0;
    mat2.data = nullptr;
    mat2.rows = nullptr;
}

// assignment
Matrix & Matrix::operator = (const Matrix & mat2) {
    if (this != & mat2) // test for self-assignment
    {
        if (mm != mat2.mm || nn != mat2.nn) {
            destroy();
            initialize(mat2.mm, mat2.nn);
        }
        copyFrom(mat2.data);
    }
    return *this;
}

// move assignment
Matrix & Matrix::operator = (Matrix && mat2) {
    destroy();
    mm = mat2.mm;
    nn = mat2.nn;
    data = mat2.data;
    rows = mat2.rows;
    mat2.mm = mat2.nn = 0;
    mat2.data = nullptr;
    mat2.rows = nullptr;
    return *this;
}

// set all elements
// use with care: no check for correct number of parameters possible
//                pass constant parameters as double, e.g. “3.0”, not “3”
Matrix & Matrix::setall(int num, ...) {
    #ifdef DEBUG
    if (num != mm * nn)
        std::cerr << "ERROR: setall with wrong number of elements" << std::endl;
    #endif
    va_list vl;
    va_start(vl, num);
    double * p {
        data
    };
    double * pend {
        data + mm * nn
    };
    for (; p < pend; ++p)
        *
        p = va_arg(vl, double);
    va_end(vl);
    return *this;
}

// output of a matrix
ostream & operator << (ostream & os,
    const Matrix & mat) {
    os << "dimension: " << mat.mm << "×" << mat.nn << '\n';
    for (int i {
            0
        }; i < mat.mm; ++i) {
        for (int j {
                0
            }; j < mat.nn; ++j)
            os << setw(9) << mat(i, j) << ' ';
        os << '\n';
    }
    os << endl;
    return os;
}

// addition of two matrices
Matrix operator + (const Matrix & mat1,
    const Matrix & mat2) {
    #ifdef DEBUG
    if (mat1.mm != mat2.mm || mat1.nn != mat2.nn)
        cerr << "ERROR: wrong dimensions with operator+" << endl;
    #endif
    Matrix res {
        mat1.mm, mat1.nn
    };
    const double * p1 {
        mat1.data
    };
    const double * p1end {
        mat1.data + mat1.mm * mat1.nn
    };
    const double * p2 {
        mat2.data
    };
    double * pr {
        res.data
    };
    for (; p1 < p1end; ++p1, ++p2, ++pr)
        *
        pr = * p1 + * p2;
    return res;
}

// subtraction of two matrices
Matrix operator - (const Matrix & mat1,
    const Matrix & mat2) {
    #ifdef DEBUG
    if (mat1.mm != mat2.mm || mat1.nn != mat2.nn)
        cerr << "ERROR: wrong dimensions with operator-" << endl;
    #endif
    Matrix res {
        mat1.mm, mat1.nn
    };
    const double * p1 {
        mat1.data
    };
    const double * p1end {
        mat1.data + mat1.mm * mat1.nn
    };
    const double * p2 {
        mat2.data
    };
    double * pr {
        res.data
    };
    for (; p1 < p1end; ++p1, ++p2, ++pr)
        *
        pr = * p1 - * p2;
    return res;
}

// multiplication scalar * matrix
Matrix operator * (double s,
    const Matrix & mat) {
    Matrix res {
        mat.mm, mat.nn
    };
    const double * p {
        mat.data
    };
    const double * pend {
        mat.data + mat.mm * mat.nn
    };
    double * pr {
        res.data
    };
    for (; p < pend; ++p, ++pr)
        *
        pr = s * * p;
    return res;
}

// multiplication of two matrices
Matrix operator * (const Matrix & mat1,
    const Matrix & mat2) {
    #ifdef DEBUG
    if (mat1.nn != mat2.mm)
        cerr << "ERROR: wrong dimensions with operator*" << endl;
    #endif
    Matrix res {
        mat1.mm, mat2.nn, 0.0
    };
    const double * p1 {
        mat1.data
    };
    const double * p2 {
        mat2.data
    };
    double * pr {
        res.data
    };
    for (int i {
            0
        }; i < mat1.mm; ++i) {
        for (int k {
                0
            }; k < mat1.nn; ++k) {
            double tmp {
                * p1++
            };
            for (int j {
                    0
                }; j < mat2.nn; ++j)
                *
                pr++ += tmp * * p2++;
            pr -= res.nn;
        }
        p2 -= mat2.mm * mat2.nn;
        pr += res.nn;
    }
    return res;
}

// comparison of two matrices
bool operator == (const Matrix & mat1,
    const Matrix & mat2) {
    if (mat1.data == mat2.data) // same data array
        return true;
    else if (mat1.mm != mat2.mm || mat1.nn != mat2.nn) // different dimensions
        return false;
    else {
        const double * p1 {
            mat1.data
        };
        const double * p1end {
            mat1.data + mat1.mm * mat1.nn
        };
        const double * p2 {
            mat2.data
        };
        for (; p1 < p1end; ++p1, ++p2)
            if ( * p1 != * p2) // at least one element is different
                return false;
    }
    return true;
}

// += implementation
void operator += (Matrix & mat1,
        const Matrix & mat2) {
    #ifdef DEBUG
    if (mat1.mm != mat2.mm || mat1.nn != mat2.nn)
        cerr << "ERROR: wrong dimensions with operator+" << endl;
    #endif
    Matrix res {
        mat1.mm, mat1.nn
    };
    const double * p1 {
        mat1.data
    };
    const double * p1end {
        mat1.data + mat1.mm * mat1.nn
    };
    const double * p2 {
        mat2.data
    };
    double * pr {
        res.data
    };
    for (; p1 < p1end; ++p1, ++p2, ++pr){
        * pr = * p1 + * p2;

        // cout << *pr << endl;
        // cout << *p1 << endl;
        // cout << *p2 << endl;
    }

    // this takes care the destroy, a better approach would be return *this 
    // but I could not implement it
    mat1 = res;
    // return res;
}

Matrix Matrix::transposed(){

    Matrix res {
        (*this).numcols(), (*this).numrows()
    };

    for (int j=0; j< (*this).numcols(); ++j){
        for (int i=0; i<(*this).numrows(); ++i){
            res(j, i) = (double) (*this)(i, j);
        }
    }

    return res;
}