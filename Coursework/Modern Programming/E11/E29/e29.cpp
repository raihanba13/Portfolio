#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>

using namespace std;

ostream& operator<< (ostream& out, pair<string, string> &pv){
    out << "First Name: " << pv.first << ", " << "Last Name: " << pv.second << endl;
    return out;
}

void print_pair_array(pair<string, string> single_pair)
{
    if(single_pair.first.at(0) == single_pair.second.at(0)){
        cout << single_pair;
    }
}

class print_pair_class {
    public:
        void operator()(const pair<string, string>& record) {
            if (record.first.at(0) == record.second.at(0)){
                cout << "First Name: " << record.first << ", " << "Last Name: " << record.second << endl;
            }
    }
};

int main()
{
    list<pair<string, string>> pair_vector;

    pair<string, string> p1;
    // first part of the pair
    p1.first = "Bkash";
    // second part of the pair
    p1.second = "Bhuiyan";

    pair_vector.push_back(p1);

    pair<string, string> p2;
    // first part of the pair
    p2.first = "Imran";
    // second part of the pair
    p2.second = "Eshan";

    pair_vector.push_back(p2);

    pair<string, string> p3;
    // first part of the pair
    p3.first = "Rafi";
    // second part of the pair
    p3.second = "Rahman";

    pair_vector.push_back(p3);

    // Function
    cout << "Function: " << endl;
    for_each(pair_vector.cbegin(), pair_vector.cend(), print_pair_array);

    auto lambda1 = [] (pair<string, string> single_pair) -> void { if(single_pair.first.at(0) == single_pair.second.at(0)){cout << single_pair ;} };
    // Lambda
    cout << "Lambda: " << endl;
    for_each(pair_vector.cbegin(), pair_vector.cend(), lambda1);

    // Function Object
    cout << "Function Object: " << endl;
    for_each(pair_vector.cbegin(), pair_vector.cend(), print_pair_class());

    return 0;
}
