// The code is working but I could not implement 0,....,n-i-2 implementation
// now it loop through all element which is not efficient
// I commented out the code that I tried 

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>

using namespace std;

template <class T>
void bubble_sort(list<T>& v){

    long unsigned count{v.size()};
    long unsigned i{0};
    long unsigned loop1{0};

    for (list<int>::iterator it{v.begin()}; it != v.end(); ++it){

        long unsigned loop2{0};
        for (list<int>::iterator it2{v.begin()}; it2 != v.end(); ++it2){


            if(*it2 > *it){
                int temp{*it2};
                *it2 = *it;
                swap(temp, *it);
            }

            // 0,....,n-i-2 implementation
            // if(loop2 == (count-loop1)){
            //     break;
            // }
            // ++loop2;

        }

        // if(loop1 == count-1){
        //     break;
        // }
        // loop1++;

    }
}

int main()
{
  list<int> v;
  v.assign({5, 1, 4, 2, 8, -1, 14, -58});

  cout << endl;


 bubble_sort(v);


  cout << "output\n" << endl;
  for (list<int>::iterator it{v.begin()}; it != v.end(); ++it){
      cout << *it << endl;
  }

}
