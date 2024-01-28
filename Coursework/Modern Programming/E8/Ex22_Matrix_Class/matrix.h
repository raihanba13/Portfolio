// class matrix, header-file
// author:  Holger Arndt
// version: 01.12.2020

#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>

#include <iostream>

class Matrix {
    private:
        int mm; // number of rows
        int nn; // number of columns
        double * data; // a pointer to the array of matrix elements
        double ** rows; // an array of pointers to the start of each row

    void initialize(int m, int n); // initialize a new (m × n)-matrix
    void destroy(); // destroy matrix
    void setelems(double val); // set all elements to val
    // replace elements by array arr, fast copy via memcpy from cstring
    void copyFrom(const double * arr) {
        memcpy(data, arr, mm * nn * sizeof(double));
    }

    public:
        // standard constructor for empty matrices
        Matrix(): mm {
            0
        }, nn {
            0
        }, data {
            nullptr
        }, rows {
            nullptr
        } {}

    // copy constructor
    Matrix(const Matrix & mat2) {
        initialize(mat2.mm, mat2.nn);
        copyFrom(mat2.data);
    }

    // move constructor
    Matrix(Matrix && mat2);

    // constructor for (m × n)-matrices, elements not initialized
    Matrix(int m, int n) {
        initialize(m, n);
    }

    // constructor for square (n × n)-matrices, elements not initialized
    explicit Matrix(int n) {
        initialize(n, n);
    }

    // constructor for (m × n)-matrices, elements initialized to val
    Matrix(int m, int n, double val) {
        initialize(m, n);
        setelems(val);
    }

    // constructor for (m × n)-matrices, initialization from an array
    Matrix(int m, int n,
            const double * arr) {
            initialize(m, n);
            copyFrom(arr);
        }

        // destructor
        ~Matrix() {
            destroy();
        }

    // access to one matrix element (returns a reference)
    double & operator()(int i, int j) {
        #ifdef DEBUG
        if (i < 0 || i >= mm || j < 0 || j >= nn)
            std::cerr << "ERROR: wrong index in operator()" << std::endl;
        #endif
        return rows[i][j];
    }

    // access to one element of a constant matrix
    double operator()(int i, int j) const {
        #ifdef DEBUG
        if (i < 0 || i >= mm || j < 0 || j >= nn)
            std::cerr << "ERROR: wrong index in operator()" << std::endl;
        #endif
        return rows[i][j];
    }

    // number of rows
    int numrows() const {
        return mm;
    }

    // number of columns
    int numcols() const {
        return nn;
    }

    // number of elements
    int numelems() const {
        return mm * nn;
    }

    // assignment
    Matrix & operator = (const Matrix & mat2);

    // move assignment
    Matrix & operator = (Matrix && mat2);

    // assignment of a constant value
    Matrix & operator = (double val) {
        setelems(val);
        return *this;
    }

    // set all elements
    Matrix & setall(int num, ...);

    // transposed
    Matrix transposed();

    // friend-declarations
    friend std::ostream & operator << (std::ostream & os,
        const Matrix & mat);
    friend Matrix operator + (const Matrix & mat1,
        const Matrix & mat2);
    friend Matrix operator - (const Matrix & mat1,
        const Matrix & mat2);
    friend Matrix operator * (double s,
        const Matrix & mat);
    friend Matrix operator * (const Matrix & mat1,
        const Matrix & mat2);
    friend bool operator == (const Matrix & mat1,
        const Matrix & mat2);
    friend void operator += (Matrix & mat1,
        const Matrix & mat2);
    // friend Matrix& operator+=(Matrix&&, Matrix mat2);
}; // end of class Matrix

// function declarations
std::ostream & operator << (std::ostream & os,
    const Matrix & mat);
Matrix operator + (const Matrix & mat1,
    const Matrix & mat2);
Matrix operator - (const Matrix & mat1,
    const Matrix & mat2);
Matrix operator * (double s,
    const Matrix & mat);
Matrix operator * (const Matrix & mat1,
    const Matrix & mat2);
bool operator == (const Matrix & mat1,
    const Matrix & mat2);
void operator += (Matrix & mat1,
        const Matrix & mat2);


// inline functions
inline Matrix operator * (const Matrix & mat, double s) {
    return s * mat;
}
inline bool operator != (const Matrix & mat1,
    const Matrix & mat2) {
    return !(mat1 == mat2);
}

#endif