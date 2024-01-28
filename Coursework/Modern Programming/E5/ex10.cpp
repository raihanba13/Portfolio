#include <iostream>

using namespace std;

float r_sum(int input_number){
    if (input_number == 0){
        return 0;
    }else{
        return input_number % 10 + r_sum(input_number/10); 
    }
}

int main(){
    int input_number;
    int sum = 0;
    cin >> input_number; 
    
    sum = r_sum(input_number);
    std::cout << sum ;
}