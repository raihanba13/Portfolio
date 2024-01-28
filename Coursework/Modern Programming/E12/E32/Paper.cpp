#include <iostream>
#include "Literature.cpp"
#include "Journal.cpp"

using namespace std;

class Paper : public Literature {
public:
    Paper();
    Paper(string Title, String Author);
    void PublishedIn(Journal* j);
    friend ostream& operator<<(ostream& os, const Paper& paper);

private:
    Journal* j;
    string author;
};

Paper::Paper(){
    j = nullptr;
    author = "Undefined author";
}

Paper::Paper(string Title, String Author) : Literature{Title} {
    author = Author;
}

void Paper::publishedIn(Journal* j)
{
     if(j != nullptr) {
         throw "The journal has already been set";
     }
     j = j;
     j->toc.push_back(this);

}

ostream& operator<<(ostream& os, const Paper& paper){
     os << "Title:  "<< paper.title << "author:  " << paper.author  << endl;
     return os;
}
