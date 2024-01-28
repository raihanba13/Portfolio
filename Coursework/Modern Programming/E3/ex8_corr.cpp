#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// we imagione the cmd as a certesian system where 
// the starting point is (b, 0)
// b is the row, a is the column
// we then start from (b,0), (b,1)...... (b, a-1)
// and if b=5, the point is like (5, 0), (5, 1).............
// and then (4, 0), (4, 1) upto a-1
// we check every point with the formula and if it is true
// we print *, otherwise space

int main() {
  int a, b; // eccentricity, number_of_space, e_x_1, e_x_2; not always integers
 double eccentricity, number_of_space, e_x_1, e_x_2; 
    // int is better here
    std::cout << "\nPlease enter a & b: " ;
    cin >> a; 
    cin >> b; 

    // for better resolution
    a *= 2;
    b *= 2;

    eccentricity = sqrt(a*a - b*b);
    // the mid line is one the x axis, they are like (e1, 0) & (e2, 0)
    e_x_1 = a - eccentricity;
    e_x_2 = a + eccentricity;

    // to give it a sharp edge
    std::cout << std::string( a-1, ' ' ); //this should come from eq
    std::cout << "*\n" ;                  //not manually inserted

    number_of_space = a - 1;
    int p_f_1, p_f_2, number_of_star;

    // we are saving this, because in negative y we will use this to print
    std::vector< int > space_l;
    std::vector< int > star_l;

    // for b, 1 is equal to 1 square
    // y axis loop
    for (int i=b; i>0; --i){
        number_of_space = 0;
        number_of_star = 0;
        
        // for a, 1 is equal to .5 square
        // x axis loop
        for (int j=0; j<a; ++j){
            p_f_1 = pow( (pow( ( e_x_1-j ), 2) + i*i ), .5) ;
            p_f_2 =  pow( (pow( ( e_x_2-j ), 2) + i*i ), .5) ;
                
            if ((p_f_1 + p_f_2) > 2*a){
                number_of_space++;
            }else{
                number_of_star++;
            } 
        }

        space_l.push_back(number_of_space);        
        star_l.push_back(number_of_star*2);        

        std::cout << std::string( number_of_space, ' ' );
        std::cout << std::string( number_of_star*2, '*' );
        std::cout << "\n";
    }

    for (int k = space_l.size()-1; k >= 0; k--) {
        std::cout << std::string( space_l.at(k), ' ' );
        std::cout << std::string( star_l.at(k), '*' );
        std::cout << "\n";
    }

    std::cout << std::string( a-1, ' ' ); 
    std::cout << "*\n" ;//should come from ellipse eq.
    
    return 0;
}
