// This does not work. Cmd keep printing some msg. 

#include <iostream>
#include "Journal.cpp"
#include "Paper.cpp"

int main(){
    string title = "Teach yourself C";
    int author = "Herbert Schildt";

    string title2 = "Teach yourself C++";
    int author2 = "Herbert Schildt";

    Paper p1(title, author);
    Paper p1(title2, author2);

    string title3 = "Book Summary on C & CPP";
    int volume = 1;

    Journal j1(title3, volume);

    cout << "Setting Journal for the first time " << endl;
    j1.publish(&p1);
    j1.publish(&p2);

    cout << "Resetting Journal ... " << endl;
    try{
        j1.publish(&p1);
    } catch(const char *e){
        cout << "Error msg: " << e << endl;
    }

    return 0;
}
