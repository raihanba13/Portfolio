#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <cmath>
#include <fstream>
#include <ctime>
#include <vector>
#include <complex>
#include <numeric>
#include <map>

using namespace std;

// global variables
const int g_N1 = 8;
const int g_N2 = 8;
// we can also square N values and get D
const int g_DN1 = g_N1*g_N1;
const int g_DN2 = g_N2*g_N2;

class Cell {
    public:
        int x;
        int y;
        int lex;
        int top_lex;
        int bottom_lex;
        int right_lex;
        int left_lex;
        bool top_lex_antiperiodic;
        bool bottom_lex_antiperiodic;
        bool right_lex_antiperiodic;
        bool left_lex_antiperiodic;
        std::complex<double> U_mu {0.0, 0.0}; 
        double U_mu_double{0.0}; 
        // possible values 1, -1, 2, -2
        int direction{1}; 

        Cell(   int x, 
                int y, 
                int lex, 
                int top_lex, 
                bool top_lex_antiperiodic, 
                int bottom_lex, 
                bool bottom_lex_antiperiodic,
                int right_lex, 
                bool right_lex_antiperiodic,
                int left_lex,
                bool left_lex_antiperiodic) 
                :   x{x}, 
                    y{y}, 
                    lex{lex}, 
                    top_lex{top_lex}, 
                    top_lex_antiperiodic{top_lex_antiperiodic}, 
                    bottom_lex{bottom_lex}, 
                    bottom_lex_antiperiodic{bottom_lex_antiperiodic}, 
                    right_lex{right_lex}, 
                    right_lex_antiperiodic{right_lex_antiperiodic}, 
                    left_lex{left_lex},
                    left_lex_antiperiodic{left_lex_antiperiodic} {};

        Cell(   int x, 
                int y, 
                int direction, 
                double U_mu_double)
                :   x{x}, 
                    y{y}, 
                    direction{direction}, 
                    U_mu_double{U_mu_double} {};

}; 

int lex_to_top_lex(int x, int y){
    // top means we go 1 direction up
    y = (y+1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_top_lex_antiperiodic(int x, int y){
    // top means we go 1 direction up
    if(y+1 == g_N1){
        return true;
    }else{
        return false;
    }
}

// TODO: This all can be placed to the constructor
int lex_to_bottom_lex(int x, int y){
    // bottom means we go 1 direction down
    y = (y-1+g_N1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_bottom_lex_antiperiodic(int x, int y){
    // bottom means we go 1 direction down
    if(y-1 == g_N1){
        return true;
    }else{
        return false;
    }
}

int lex_to_right_lex(int x, int y){
    // right means we go 1 direction in the positive x
    x = (x+1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_right_lex_antiperiodic(int x, int y){
    // right means we go 1 direction in the positive x
    if(x+1 == g_N1){
        return true;
    }else{
        return false;
    }
}

int lex_to_left_lex(int x, int y){
    // left means we go 1 direction in the negative x
    x = (x-1+g_N1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_left_lex_antiperiodic(int x, int y){
    // left means we go 1 direction in the negative x
    if(x-1 == g_N1){
        return true;
    }else{
        return false;
    }
}

vector<double> mv_dot(vector<double> A, vector<double> x, int A_row, int A_col, int x_col){
    int i, j;
    vector<double> b(A_row*x_col, 0.0);
    
    int k{0};
    for(i=0, j=0; i<A_row*A_col; i++){
        b[k] += A[i] * x[j];
        j++;
        if(j == A_col){
            j = 0;
            k++;
        }
    }

    return b;

}

// not memory efficient, but does the work
vector<double> mm_dot2(vector<double> A, vector<double> x, int A_row, int A_col, int x_col){
    vector<double> b(A_row*x_col, 0.0);
    int i, j, k;
    auto a_start = A.begin();

    vector<double> temp_a_row(A_row, 0.0);
    // vector<double> temp_x_col(A_col, 0.0);
    vector<double> temp_x_col;
    vector<double> temp_b(1, 0.0);

    for(i=0; i<A_row; i++){
        temp_a_row = vector<double>(a_start, a_start+A_col);
        for(j=0; j<x_col; j++){
            // we have to construct x_col
            // There could be a nice cpp way to do this things
            for(k=0; k<A_col; k++){
                temp_x_col.push_back(x[k*x_col+j]);
            }
            // print for debugging 

            // cout << "Printing A row " << endl;
            // for(double x: temp_a_row){
            //     cout << "elem of A : " << x << endl;
            // }
            // cout << "Printing x col " << endl;
            // for(double x: temp_x_col){
            //     cout << "elem of x : " << x << endl;
            // }
            // cout << "Now we do the mv_dot ..." << endl;


            temp_b = mv_dot(temp_a_row, temp_x_col, A_row, A_col, 1);
            // cout << temp_b[0] << endl;
            b[i*x_col+j] = temp_b[0];
            temp_x_col.clear();
            
        }
        a_start += A_col;
    }

    return b;
}

// /*

double D_val(int point_x_lex, int point_y_lex, const vector<Cell>& lattice, bool conjugate_transpose=false){
    // variable
    double m = 0.4;
    // conditional delta
    // r=>right(+1), l=>left(-1), t=>top(+2), b=>bottom(-2)
    double delta_x_y{0.0};
    double delta_x_r_y{0.0};
    double delta_x_l_y{0.0};
    double delta_x_t_y{0.0};
    double delta_x_b_y{0.0};
    double result{0.0};

    // it mentioned that U_n(x) = 1 for simplicity 
    // I am not 100% sure about the value of U1_star_x_l, U2_star_x_b
    // std::complex<double> U_mu {0.0, 0.0}; 
    double U1_x{1.00};
    double U2_x{1.00};
    double U1_star_x_l{1.00};
    double U2_star_x_b{1.00};

    // check for delta_x_y
    if (point_x_lex == point_y_lex){
        delta_x_y = 1.0;
    }

    // check for delta_x_r_y
    if (lattice[point_x_lex].right_lex == point_y_lex){
        delta_x_r_y = 1.0;
        if(lattice[point_x_lex].right_lex_antiperiodic){
            delta_x_r_y = -1.0;
        }
    }

    // check for delta_x_l_y
    if (lattice[point_x_lex].left_lex == point_y_lex){
        delta_x_l_y = 1.0;
        if(lattice[point_x_lex].left_lex_antiperiodic){
            delta_x_l_y = -1.0;
        }
    }

    // check for delta_x_t_y
    if (lattice[point_x_lex].top_lex == point_y_lex){
        delta_x_t_y = 1.0;
        if(lattice[point_x_lex].top_lex_antiperiodic){
            delta_x_t_y = -1.0;
        }
    }

    // check for delta_x_b_y
    if (lattice[point_x_lex].bottom_lex == point_y_lex){
        delta_x_b_y = 1.0;
        if(lattice[point_x_lex].bottom_lex_antiperiodic){
            delta_x_b_y = -1.0;
        }
    }

    result = m * delta_x_y +
                    (U1_x * delta_x_r_y - U1_star_x_l * delta_x_l_y) / 2.0 +
                    pow((-1), lattice[point_x_lex].x) * (U2_x * delta_x_t_y - U2_star_x_b * delta_x_b_y) / 2.0;    
    
    return result;
}

double D_val_conjugate_transpose(int point_x_lex, int point_y_lex, const vector<Cell>& lattice){
    // variable
    double m = 0.4;
    // conditional delta
    // r=>right(+1), l=>left(-1), t=>top(+2), b=>bottom(-2)
    double delta_x_y{0.0};
    double delta_x_y_l{0.0};
    double delta_x_y_r{0.0};
    double delta_x_y_b{0.0};
    double delta_x_y_t{0.0};
    double result{0.0};

    // it mentioned that U_n(x) = 1 for simplicity 
    // I am not 100% sure about the value of U1_star_x_l, U2_star_x_b
    // we can use a map with all this value, take input and also apply conjugate
    // std::complex<double> U1_x {1.0, 0.0}; 
    // std::complex<double> U2_x {1.0, 0.0}; 
    // std::complex<double> U1_star_x_l {1.0, 0.0}; 
    // std::complex<double> U2_star_x_b {1.0, 0.0}; 
    double U1_x{1.00};
    double U2_x{1.00};
    double U1_star_x_l{1.00};
    double U2_star_x_b{1.00};

    // check for delta_x_y
    if (point_x_lex == point_y_lex){
        delta_x_y = 1.0;
    }

    // check for delta_x_y_l
    if (lattice[point_x_lex].lex == lattice[point_y_lex].bottom_lex){
        delta_x_y_l = 1.0;
        if(lattice[point_y_lex].bottom_lex_antiperiodic){
            delta_x_y_l = -1.0;
        }
    }

    // check for delta_x_y_r
    if (lattice[point_x_lex].lex == lattice[point_y_lex].right_lex){
        delta_x_y_r = 1.0;
        if(lattice[point_y_lex].right_lex_antiperiodic){
            delta_x_y_r = -1.0;
        }
    }

    // check for delta_x_y_b
    if (lattice[point_x_lex].lex == lattice[point_y_lex].bottom_lex){
        delta_x_y_b = 1.0;
        if(lattice[point_y_lex].bottom_lex_antiperiodic){
            delta_x_y_b = -1.0;
        }
    }

    // check for delta_x_y_t
    if (lattice[point_x_lex].lex == lattice[point_y_lex].top_lex){
        delta_x_y_t = 1.0;
        if(lattice[point_y_lex].top_lex_antiperiodic){
            delta_x_y_t = -1.0;
        }
    }

    result = m * delta_x_y +
                    (U1_x * delta_x_y_l - U1_star_x_l * delta_x_y_r) / 2.0 +
                    pow((-1), lattice[point_x_lex].x) * (U2_x * delta_x_y_b - U2_star_x_b * delta_x_y_t) / 2.0;
    
    return result;
}

// */
vector<double> mv_D_v(int g_DN1, int g_DN2, vector<double> v, vector<Cell> lattice){
    vector<double> new_v(g_DN1, 0.0);
    int i, j;
    double d_x_y;

    // calculate D* dot v
    for(i=0; i<g_DN1; i++){
        for(j=0; j<g_DN1; j++){
            if(v[j] != 0.0){
                d_x_y = D_val(i, j, lattice, true)*v[j];
            }
        }
        new_v[i] = d_x_y;
        // cout << "idx: " << i << ", value= " << d_x_y << endl;
    }

    return new_v;
}

vector<double> mv_DConTrans_v(int g_DN1, int g_DN2, vector<double> v, vector<Cell> lattice){
    vector<double> new_v(g_DN1, 0.0);
    int i, j;
    double d_x_y;

    // calculate D* dot v
    for(i=0; i<g_DN1; i++){
        for(j=0; j<g_DN1; j++){
            if(v[j] != 0.0){
                d_x_y = D_val_conjugate_transpose(i, j, lattice)*v[j];
            }
        }
        new_v[i] = d_x_y;
        // cout << "idx: " << i << ", value= " << d_x_y << endl;
    }

    return new_v;
}

vector<double> LinearCG(vector<double> A, vector<double> b, vector<double> x0, int A_row, int A_col, int x_col){
    // creating a new copy of x0, this is eq of deepcopy
    vector<double> xk = x0;
    vector<double> rk = mv_dot(A, xk, A_row, A_col, x_col);
    double tol = 1e-5;
    // matrix subtract
    // 4th parameter is the variable that will be used to store the new value
    // use something like std::back_inserter(results) where result is a vector if you want to store data in a new var
    transform(rk.begin(), rk.end(), b.begin(), rk.begin(), [](double a, double b) { return a-b; });
    
    // now we have rk = (A dot xk) - b

    // pk = -rk
    vector<double> pk(A_col*x_col, 0.0);
    transform(rk.begin(), rk.end(), pk.begin(), [](double a) { return -a; });

    double rk_norm = sqrt(inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ));

    // cout << rk_norm << endl; 

    int num_iter = 0;
    vector<double> apk(A_col*x_col, 0.0);
    double rkrk, alpha;
    vector<double> alpha_mul_pk(A_col);
    vector<double> alpha_mul_apk(A_col);
    while (rk_norm > tol){

        apk = mv_dot(A, pk, A_row, A_col, x_col);
        rkrk = inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 );
        alpha = rkrk / inner_product( pk.begin(), pk.end(), apk.begin(), 0.0 );

        // this is a scalar matrix mul, TODO: transform to a func 
        transform(pk.begin(), pk.end(), alpha_mul_pk.begin(), [&alpha](double a) { return alpha*a; });
        // xk = transform(pk.begin(), pk.end(), alpha_mul_pk.begin(), [](double a, double b) { return a+b; });
        transform(xk.begin(), xk.end(), alpha_mul_pk.begin(), xk.begin(), [](double a, double b) { return a+b; });

        transform(apk.begin(), apk.end(), alpha_mul_apk.begin(), [&alpha](double a) { return alpha*a; });
        // rk = transform(rk.begin(), rk.end(), alpha_mul_apk.begin(), [](double a, double b) { return a+b; });
        transform(rk.begin(), rk.end(), alpha_mul_apk.begin(), rk.begin(), [](double a, double b) { return a+b; });

        double beta = inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ) / rkrk;

        transform(pk.begin(), pk.end(), pk.begin(), [&beta](double a) { return a*beta; });
        transform(rk.begin(), rk.end(), pk.begin(), pk.begin(), [](double a, double b) { return -a+b; });
        

        num_iter++;
        rk_norm = sqrt(inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ));

        // force break after 100 iter
        // cout << "Iter count: " << num_iter << endl;
        // cout << "tol: " << tol << endl;
        // cout << "rk_norm: " << rk_norm << endl;
        // cout << "=================================" << endl;
        if(num_iter >= 100){
            break;
        }

        /*
        */
    }

    return xk;

}


vector<double> LinearCG_nonSPD(vector<double> b, vector<double> x0, vector<Cell> lattice, int A_row, int A_col, int x_col){
    // creating a new copy of x0, this is eq of deepcopy
    // A_row should be g_DN1
    // A_col should be g_DN2
    // x_col should be 1
    vector<double> xk = x0;
    // changed
    // vector<double> rk = mv_dot(A, xk, A_row, A_col, x_col);
    // A will be changed to A* dot A and the final product is A * dot A dot xk
    vector<double> rk(A_col, 0.0);
    rk = mv_D_v(g_DN1, g_DN2, xk, lattice);
    rk = mv_DConTrans_v(g_DN1, g_DN2, rk, lattice);
    double tol = 1e-5;
    // matrix subtract
    // 4th parameter is the variable that will be used to store the new value
    // use something like std::back_inserter(results) where result is a vector if you want to store data in a new var
    transform(rk.begin(), rk.end(), b.begin(), rk.begin(), [](double a, double b) { return a-b; });
    
    // now we have rk = (A dot xk) - b

    // pk = -rk
    vector<double> pk(A_col, 0.0);
    transform(rk.begin(), rk.end(), pk.begin(), [](double a) { return -a; });

    double rk_norm = sqrt(inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ));

    // cout << rk_norm << endl; 

    int num_iter = 0;
    vector<double> apk(A_col, 0.0);
    double rkrk, alpha;
    vector<double> alpha_mul_pk(A_col, 0.0);
    vector<double> alpha_mul_apk(A_col, 0.0);
    while (rk_norm > tol){

        // apk = mv_dot(A, pk, A_row, A_col, x_col);
        apk = mv_D_v(g_DN1, g_DN2, pk, lattice);
        apk = mv_DConTrans_v(g_DN1, g_DN2, apk, lattice);

        rkrk = inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 );
        alpha = rkrk / inner_product( pk.begin(), pk.end(), apk.begin(), 0.0 );

        // this is a scalar matrix mul, TODO: transform to a func 
        transform(pk.begin(), pk.end(), alpha_mul_pk.begin(), [&alpha](double a) { return alpha*a; });
        // xk = transform(pk.begin(), pk.end(), alpha_mul_pk.begin(), [](double a, double b) { return a+b; });
        transform(xk.begin(), xk.end(), alpha_mul_pk.begin(), xk.begin(), [](double a, double b) { return a+b; });

        transform(apk.begin(), apk.end(), alpha_mul_apk.begin(), [&alpha](double a) { return alpha*a; });
        // rk = transform(rk.begin(), rk.end(), alpha_mul_apk.begin(), [](double a, double b) { return a+b; });
        transform(rk.begin(), rk.end(), alpha_mul_apk.begin(), rk.begin(), [](double a, double b) { return a+b; });

        double beta = inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ) / rkrk;

        transform(pk.begin(), pk.end(), pk.begin(), [&beta](double a) { return a*beta; });
        transform(rk.begin(), rk.end(), pk.begin(), pk.begin(), [](double a, double b) { return -a+b; });
        

        num_iter++;
        rk_norm = sqrt(inner_product( rk.begin(), rk.end(), rk.begin(), 0.0 ));

        // force break after 100 iter
        // cout << "Iter count: " << num_iter << endl;
        // cout << "tol: " << tol << endl;
        // cout << "rk_norm: " << rk_norm << endl;
        // cout << "=================================" << endl;
        if(num_iter >= 100){
            break;
        }

        // printing xk to debug
        /*
        cout << "printing xk ..." << endl;
        for(double x: xk){
            cout << x << endl;
        }
        cout << "printing xk ends ..." << endl;
        cout << "=================================" << endl;

        
        */
    }

    return xk;

}

// overloading cout for vector to make it easier
ostream& operator<<(ostream& os, const vector<double> v){
    for(double x: v){
        cout << x << endl;
    }
    return os;
}

void elegent_matrix_printer(const vector<double> v, int col){
    int i{0};
    for(double x: v){
        cout << x << ", " ;
        i++;
        if(i%col == 0){
            cout << endl;
        }
    }
}




int main(){
    // task a starts, we construct 4X4 matrix
    vector<Cell> lattice;

    int x, y;
    int i, j;

    for(y=0; y<g_N1; y++){
        for(x=0; x<g_N2; x++){
            Cell c (
                x, 
                y,
                x+y*g_N1,
                lex_to_top_lex(x, y),
                f_top_lex_antiperiodic(x, y),
                lex_to_bottom_lex(x, y),
                f_bottom_lex_antiperiodic(x, y),
                lex_to_right_lex(x, y),
                f_right_lex_antiperiodic(x, y),
                lex_to_left_lex(x, y),
                f_left_lex_antiperiodic(x, y)
            );

            lattice.push_back(
                    c
                );
            
        }
    }

    /*
    // debug: to print all the point of the lattice
    for(Cell x : lattice){
        cout << "x: " << x.x << endl;
        cout << "y: " << x.y << endl;
        cout << "lex: " << x.lex << endl;
        cout << "top (lex): " << x.top_lex << endl;
        cout << "bottom (lex): " << x.bottom_lex << endl;
        cout << "right (lex): " << x.right_lex << endl;
        cout << "left (lex): " << x.left_lex << endl;
        cout << "U_mu: " << real(x.U_mu) << " + " << imag(x.U_mu) << "i" << endl;
        cout << "=========================" << endl;
    }
    */

    vector<double> v(g_DN1, 0.0);

    // if we want to do it random, for testing I keep it hard coded
    // srand(std::time(nullptr));
    // int random_variable = rand()%g_DN1;
    // cout << "Random variable: " << random_variable << endl;
    // v[random_variable] = 1.0;
    v[5] = 1.0;

    ofstream ofile_pointer ( "D_data.csv" );
    double d_x_y{0.0};

    // We are constructing D, D_CT to do a quick check of our CG routine
    vector<double> D_Matrix(g_DN1*g_DN2, 0.0);
    // Conjugate does not matter since D is double
    vector<double> D_CT_Matrix(g_DN1*g_DN2, 0.0);
    for(i=0; i<g_DN1; i++){
        for(j=0; j<g_DN2; j++){
            d_x_y = D_val(i, j, lattice);
            // cout << "x: " << i << endl;
            // cout << "y: " << j << endl;
            // cout << "D_x_y: " << d_x_y << endl;
            // cout << "=================" << endl;
            // writing D in a file so we can visualize later
            // TODO: now python will do the visualization
            ofile_pointer << d_x_y << ",";
            D_Matrix[j+i*g_DN1] = d_x_y;
            D_CT_Matrix[j*g_DN1+i] = d_x_y;
        }
        ofile_pointer << "\n";
    }
    ofile_pointer.close();

    cout << "printint D ..." << endl;
    elegent_matrix_printer(D_Matrix, g_DN1);
    cout << "printint D_CT ..." << endl;
    elegent_matrix_printer(D_CT_Matrix, g_DN1);

    vector<double> D_CT_Matrix_D(g_DN1*g_DN2, 0.0);

    D_CT_Matrix_D = mm_dot2(D_CT_Matrix, D_Matrix, g_DN1, g_DN2, g_DN2);
    vector<double> D_CT_v(g_DN1, 0.0);
    D_CT_v = mv_dot(D_CT_Matrix, v, g_DN1, g_DN2, 1);

    // initial x
    vector<double> x0(g_DN1, 2.3);

    vector<double> xk(g_DN1, 0.0);
    xk = LinearCG(D_CT_Matrix_D, D_CT_v, x0, g_DN1, g_DN2, 1);

    cout << "\n\nPrinting result ..." << endl;
    cout << xk;


    // Now we check the LinearCG_nonSPD routine 
    vector<double> D_star_dot_v(g_DN1, 0.0);
    D_star_dot_v = mv_DConTrans_v(g_DN1, g_DN2, v, lattice);

    vector<double> D_inverse_e_i(g_DN1, 0.0);
    D_inverse_e_i = LinearCG_nonSPD(D_star_dot_v, x0, lattice, g_DN1, g_DN2, 1);

    cout << "\n\nPrinting result with non SPD routine ..." << endl;
    cout << D_inverse_e_i;

    // comment: CG routine is working with constructing D and D* explicitly
    // CG_nonSPD is not working

    cout << "Task a-d is finished.\nTask e starts here.\n";
    // we have a v and hardcoded 6 position with 1
    // Calculating C
    // xk is valid result

    cout << "\n\nPrinting result ..." << endl;
    cout << xk;

    vector<double> C_t;
    int t;

    // we save the result, its not memory efficient but its computational efficient
    vector<double> e_i(g_DN1, 0.0);
    map<int, vector<double>> inD_e_i;
    for(i=0; i<g_DN1; i++){
        e_i[i] = 1.0; 
        D_CT_v = mv_dot(D_CT_Matrix, e_i, g_DN1, g_DN2, 1);
        xk = LinearCG(D_CT_Matrix_D, D_CT_v, x0, g_DN1, g_DN2, 1);

        inD_e_i.insert(
            make_pair(
                i, xk
            )
        );

        if(i != 0){
            e_i[i-1] = 0.0;
        }

    }

    ofstream ofile_c_pointer ( "C_data.csv" );
    double d_inv_e_i_term{0.0};
    for(t=0; t<g_N2-1; t++){
        for(j=0; j<g_N1; j++){
            for(i=0; i<g_N2; i++){
                xk = inD_e_i[i + (j-1) * g_N2];
                d_inv_e_i_term += xk[((i + (j-1) * g_N2)+t)%g_DN1]*xk[((i + (j-1) * g_N2)+t)%g_DN1];
            }
        }
        ofile_c_pointer << d_inv_e_i_term/((double)g_N1*g_N2) << ",";
        d_inv_e_i_term = 0.0;
    }


    // task f, we load the data
    cout << "Task f starts here.\n";

    ifstream D_example_file("example_for_U.dat.txt");
    // i=>column or x, j=>row or y, x=>direction, d_x_y is the value of U
    // we are shifting -1 as our system starts from 0
    vector<Cell*> D_cells;
    while (D_example_file >> i >> j >> x >> d_x_y){
        D_cells.push_back(
                    new Cell(
                        i-1, j-1, x, d_x_y
                    )
                );
    }

    cout << "We loaded the data.\n";

    return 0;
}