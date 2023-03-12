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
# include <mpi.h>
#include <map>

using namespace std;

// global variables
const int g_N1 = 8;
const int g_N2 = 8;
// we can also square N values and get D
const int g_DN1 = 64;
const int g_DN2 = 64;

struct GRID_DATA {
    int p;
    MPI_Comm comm;
    MPI_Comm row_comm;
    MPI_Comm col_comm;
    int q;
    int my_row;
    int my_col;
    int my_rank;
};


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
        complex<double> U_mu {0.0, 0.0}; 
        double U_mu_double {0.0};

        // We keep U_mu_double to simplify things for the moment

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

}; 

int lex_to_bottom_lex(int x, int y){
    // top means we go 1 direction up
    y = (y+1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_bottom_lex_antiperiodic(int x, int y){
    // top means we go 1 direction up
    if(y+1 == g_N1){
        return true;
    }else{
        return false;
    }
}

// TODO: This all can be placed to the constructor
int lex_to_top_lex(int x, int y){
    // bottom means we go 1 direction down
    y = (y-1+g_N1)%g_N1;
    // generate the lex and return in 
    return x + y * g_N1;
}

bool f_top_lex_antiperiodic(int x, int y){
    // bottom means we go 1 direction down
    if(y-1 == g_N1){
        return true;
    }else{
        return false;
    }
}

int lex_to_right_lex(int x, int y){
    // right means we go 1 direction in the positive x
    x = (x+1)%g_N2;
    // generate the lex and return in 
    return x + y * g_N2;
}

bool f_right_lex_antiperiodic(int x, int y){
    // right means we go 1 direction in the positive x
    if(x+1 == g_N2){
        return true;
    }else{
        return false;
    }
}

int lex_to_left_lex(int x, int y){
    // left means we go 1 direction in the negative x
    x = (x-1+g_N2)%g_N2;
    // generate the lex and return in 
    return x + y * g_N2;
}

bool f_left_lex_antiperiodic(int x, int y){
    // left means we go 1 direction in the negative x
    if(x-1 == g_N2){
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
// ostream& operator<<(ostream& os, const vector<double>& v){
//     for(double x: v){
//         cout << x << endl;
//     }
//     return os;
// }

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

ostream & operator <<(ostream &os, const map<int, Cell*> &lattice_container)
{
    for(const auto &p : lattice_container){

        os << "~~~\nLex: " << p.first << ": \n";

        os << "x: " << p.second->x << endl;
        os << "y: " << p.second->y << endl;
        os << "lex: " << p.second->lex << endl;
        os << "top (lex): " << p.second->top_lex << endl;
        os << "bottom (lex): " << p.second->bottom_lex << endl;
        os << "right (lex): " << p.second->right_lex << endl;
        os << "left (lex): " << p.second->left_lex << endl;
        os << "U_mu: " << real(p.second->U_mu) << " + " << imag(p.second->U_mu) << "i\n~~~" << endl;

    }

    return os;
}

int main(int argc, char *argv[]){

    // id is the current processor rank
    int id;
    // total processor number
    int p;
    double wtime;
    int m, n;
    // x is column, y is row
    int x, y;
    int i;

    MPI_Init ( &argc, &argv );
    MPI_Comm_rank ( MPI_COMM_WORLD, &id );
    MPI_Comm_size ( MPI_COMM_WORLD, &p );

    // for(i=0; i<p; i++){
    //     if(id == i){
    //         cout << "Rank: " << id << endl;
    //     }
    // }

    // we need a square matrix to distribute everything
    // if(id == 0 && (p % g_N1 != 0 || p % g_N2 != 0)){
    //     cout << p << endl;
    //     cout << g_N1 << endl;
    //     cout << g_N2 << endl;
    //     cout << (p % g_N1) << endl;
    //     cout << (p % g_N2) << endl;
    //     cout << "number of processor is not compatiable with the problem size!" << endl;
    //     exit(-1);
    // }

    // we set up the grid
    GRID_DATA grid;

    int dimensions[2];
	int wrap_around[2];
	int coordinates[2];
	int free_coords[2];

    grid.q = (int) sqrt((double) p);
    dimensions[0] = dimensions[1] = grid.q;

    wrap_around[0] = wrap_around[1] = 1;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions,
		wrap_around, 1, &(grid.comm));
	MPI_Comm_rank(grid.comm, &(grid.my_rank));
	MPI_Cart_coords(grid.comm, grid.my_rank, 2,
		coordinates);
	grid.my_row = coordinates[0];
	grid.my_col = coordinates[1];

	/*Set up row communicators */
	free_coords[0] = 0;
	free_coords[1] = 1;
	MPI_Cart_sub(grid.comm, free_coords, &(grid.row_comm));

	/*Set up column communicators */
	free_coords[0] = 1;
	free_coords[1] = 0;
	MPI_Cart_sub(grid.comm, free_coords, &(grid.col_comm));

    // set up grid is finished

    int local_N1 = g_N1 / (p/2);
    int local_N2 = g_N2 / (p/2);

    // cout << local_N1 << ", " << local_N1 << endl;

    // exit(-1);

    // key represents the lattice number
    // in previous version the index of lattice list does the same
    // more memory, easier implementation
    map<int, Cell*> lattice_container;
    int lex;


    // generate local cell 
    // vector<Cell> lattice;
    for(y=0; y<local_N1; y++){
        for(x=0; x<local_N2; x++){
            // lex = n + grid.my_row*local_N1 + grid.my_col*local_N2 + m*g_N2;

            

            // m is row or y, n is column or x
            n = x + grid.my_col*local_N2;
            m = y + grid.my_row*local_N1;
            lex = m + n*g_N2;

            if(!id){
                cout << n << ", " << m << endl;
            }

            lattice_container.insert(
                                make_pair(
                                            lex, 
                                            new Cell(
                                                n, 
                                                m,
                                                lex,
                                                lex_to_top_lex(n, m),
                                                f_top_lex_antiperiodic(n, m),
                                                lex_to_bottom_lex(n, m),
                                                f_bottom_lex_antiperiodic(n, m),
                                                lex_to_right_lex(n, m),
                                                f_right_lex_antiperiodic(n, m),
                                                lex_to_left_lex(n, m),
                                                f_left_lex_antiperiodic(n, m)
                                            )
                                        )
                                    );
        }
    }

    if(!id){
        cout << "local point ends ..." << endl;
    }

    // now we collect the bondary lattice cells
    // top 

    m = (grid.my_row*local_N1 - 1 + g_N1) % g_N1;
    for(x=0; x<local_N1; x++){
        
        n = x + grid.my_col*local_N2;
        lex = n + m*g_N2;

        // if(id == 3){
        //     cout << lex << ": " << n << ", " << m << endl;
        // }

        lattice_container.insert(
                            make_pair(
                                        lex, 
                                        new Cell(
                                            n, 
                                            m,
                                            lex,
                                            lex_to_top_lex(n, m),
                                            f_top_lex_antiperiodic(n, m),
                                            lex_to_bottom_lex(n, m),
                                            f_bottom_lex_antiperiodic(n, m),
                                            lex_to_right_lex(n, m),
                                            f_right_lex_antiperiodic(n, m),
                                            lex_to_left_lex(n, m),
                                            f_left_lex_antiperiodic(n, m)
                                        )
                                    )
                                );
        
    }

    // bottom

    m = (grid.my_row*local_N1 + local_N1 ) % g_N1;
    for(x=0; x<local_N1; x++){
        
        n = x + grid.my_col*local_N2;
        lex = n + m*g_N2;

        // if(id == 2){
        //     cout << lex << ": " << n << ", " << m << endl;
        // }

        lattice_container.insert(
                            make_pair(
                                        lex, 
                                        new Cell(
                                            n, 
                                            m,
                                            lex,
                                            lex_to_top_lex(n, m),
                                            f_top_lex_antiperiodic(n, m),
                                            lex_to_bottom_lex(n, m),
                                            f_bottom_lex_antiperiodic(n, m),
                                            lex_to_right_lex(n, m),
                                            f_right_lex_antiperiodic(n, m),
                                            lex_to_left_lex(n, m),
                                            f_left_lex_antiperiodic(n, m)
                                        )
                                    )
                                );
        
    }

    // left

    n = (grid.my_col*local_N2 -1 + g_N2 ) % g_N2;
    for(x=0; x<local_N1; x++){
        
        m = x + grid.my_row*local_N1;
        lex = n + m*g_N2;

        // if(id == 3){
        //     cout << lex << ": :=) " << n << ", " << m << endl;
        // }

        lattice_container.insert(
                            make_pair(
                                        lex, 
                                        new Cell(
                                            n, 
                                            m,
                                            lex,
                                            lex_to_top_lex(n, m),
                                            f_top_lex_antiperiodic(n, m),
                                            lex_to_bottom_lex(n, m),
                                            f_bottom_lex_antiperiodic(n, m),
                                            lex_to_right_lex(n, m),
                                            f_right_lex_antiperiodic(n, m),
                                            lex_to_left_lex(n, m),
                                            f_left_lex_antiperiodic(n, m)
                                        )
                                    )
                                );
        
    }

    // right

    n = (grid.my_col*local_N2 + local_N2 ) % g_N2;
    for(x=0; x<local_N1; x++){
        
        m = x + grid.my_row*local_N1;
        lex = n + m*g_N2;

        if(id == 0){
            cout << lex << ": :=) " << n << ", " << m << endl;
        }

        lattice_container.insert(
                            make_pair(
                                        lex, 
                                        new Cell(
                                            n, 
                                            m,
                                            lex,
                                            lex_to_top_lex(n, m),
                                            f_top_lex_antiperiodic(n, m),
                                            lex_to_bottom_lex(n, m),
                                            f_bottom_lex_antiperiodic(n, m),
                                            lex_to_right_lex(n, m),
                                            f_right_lex_antiperiodic(n, m),
                                            lex_to_left_lex(n, m),
                                            f_left_lex_antiperiodic(n, m)
                                        )
                                    )
                                );
        
    }

    

    



    // print the local map to debug
    // for(i=0; i<p; i++){
    //     if(id == 0){
    //     // if(id == i){
    //         cout << "\n=================================\n" ;
    //         cout << "id: " << id << endl;
    //         cout << "my_row: " << grid.my_row << endl;
    //         cout << "my_col: " << grid.my_col << endl;
    //         cout << "my_rank: " << grid.my_rank << endl;
    //         cout << "\n#################################################\n" ;
    //         cout << lattice_container;
    //         cout << "\n#################################################" ;
    //         cout << "\n=================================\n" ;
    //     }
    // }

    // if(id == 3){
    //     cout << "\n=================================\n" ;
    //     cout << "id: " << id << endl;
    //     cout << "my_row: " << grid.my_row << endl;
    //     cout << "my_col: " << grid.my_col << endl;
    //     cout << "my_rank: " << grid.my_rank << endl;
    //     cout << "\n#################################################\n" ;
    //     cout << lattice_container;
    //     cout << "\n#################################################" ;
    //     cout << "\n=================================\n" ;
    // }




    MPI_Finalize();
    
    return 0;
}