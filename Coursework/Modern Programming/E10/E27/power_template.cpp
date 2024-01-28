#include <iostream>
#include <cctype>
#include <string>

using namespace std;

template <class T>
T power_template(T a, unsigned int n){

    if(n == 0){
        return 1;
    }
    auto result = a;
    for(int i=0; i<n-1; i++){
        result *= a;
    }

    return result;
}

template<>
string power_template(string a, unsigned int n){
    // 0 should not be a valid input for string
    string result = a;
    for(int i=0; i<n-1; i++){
        result = result + a;
    }

    return result;
}

int main(){
    cout << "Testing int & float: " << endl;
    cout << power_template(3, 0) << endl;
    cout << power_template(3, 1) << endl;
    cout << power_template(3, 4) << endl;
    cout << power_template(4.0, 3.0) << endl;

    cout << "Testing string: " << endl;
    cout << power_template(string("ABC"), 3.0) << endl;
    cout << power_template(string("CPP"), 2) << endl;
}
