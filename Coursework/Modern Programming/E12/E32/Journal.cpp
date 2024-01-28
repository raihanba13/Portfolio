#include <list>
#include "Literature.cpp"
#include "Paper.cpp"
#include <iostream>

using namespace std;

class Journal : public Literature{
public:
    Journal() {};
    Journal(string Title, int Volume) : Literature{Title} {
        volume = Volume;
    };
    void publish(Paper *p);
    void printToc();

    list<*Paper> toc;
    int volume;
};

void Journal::publish(Paper *p){
    p->publishedIn(*this);
}

void Journal::printToc(){
    for (list<*Paper>::iterator it{toc.begin()}; it != toc.end(); ++it){
        cout << *it->title << endl;
        cout << *it->author << endl;
    }
}
