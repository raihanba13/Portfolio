#include <iostream>

using namespace std;

class Literature{
public:
    string title;
    Literature() : title{"Undefined Title"} {};
    Literature(string Title) : title{Title} {};
};
