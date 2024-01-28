#include <iostream>
#include <math.h>

using namespace std;

int main() {
  while (1){
    int user_input;
    std::cout << "\nPlease enter a number: " ;
    cin >> user_input; 

    if(user_input >= 0){
      int s_root;
      s_root = sqrt(user_input);
      int prime_flag = 0;

      for (int i = 1; i <=user_input; ++i){
        if (user_input % i == 0) {
          std::cout << "\n" ;
          std::cout << user_input/i << "*" << i ;
          prime_flag++;
        }
      }

      std::cout << "\n";
      if (prime_flag > 1){
        std::cout << user_input <<" is not a prime number." ;
      }else{
        std::cout << user_input <<" is a prime number." ;
      }
    }else if (user_input<0){ 
      std::cout << "Program terminated." ;
      return 0;
    }
  }
    
  return 0;
}
