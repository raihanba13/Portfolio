#include <iostream>
#include <math.h>

using namespace std;

const float PI = 3.14159265358979323846;

float calc_cuboid_vol(float a, float b, float c){
    return a*b*c;
}

float calc_cube_vol(float a){
    return a*a*a;
}

float calc_sphere_vol(float r){
    return (4/3.0)*PI*r*r*r;
}

int main() {

    while (1){
    	std::cout << "(c) cuboid\n";
    	std::cout << "\tParameters:\t side lenghts a, b, and c\n";
    	std::cout << "\tVolume:\t a . b. c\n";

        std::cout << "(C) cube\n";
    	std::cout << "\tParameters:\t side lenght a\n";
    	std::cout << "\tVolume:\t a^3\n";

        std::cout << "(s) sphere\n";
    	std::cout << "\tParameters:\t radius r\n";
    	std::cout << "\tVolume:\t (4/3)*pi*r^3)\n";

        std::cout << "(x) exit the program\n";

        std::string user_input;

        std::cout << "Please enter your choice: \n";
        cin >> user_input; 

        if (user_input == "c"){
            std::cout << "Side Lenghts a, b, c: ";
            float a, b, c, volume;
            cin >> a; 
            cin >> b; 
            cin >> c; 
            volume = calc_cuboid_vol(a,b,c);
            std::cout << "\nVolume of Cuboid: " <<volume << "\n\n\n";
        }else if (user_input == "C"){
            std::cout << "Side Lenght a: ";
            float a, volume;
            cin >> a; 
            volume = calc_cube_vol(a);
            std::cout << "\nVolume of Cuboid: " <<volume << "\n\n\n";
        }else if (user_input == "s"){           
            std::cout << "Radius r: ";
            float r, volume;
            cin >> r; 
            volume = calc_sphere_vol(r);
            std::cout << "\nVolume of Sphere: " <<volume << "\n\n\n";
        }else if(user_input == "x"){
                std::cout << "\n\nProgram terminated successfully.";
                return 0;
        }else{
            std::cout << "\nPlease choose correctly.";
        }  
                

	}
}
