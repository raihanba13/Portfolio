// this is not completed and the arithmetic operation has some bugs
#include<iostream>
using namespace std;

template<class T>
class Interval{
    private:
        T inf;
        T sup;
    public:
        Interval(T start,T end) {
             inf = start;
             sup = end;
        }
        Interval(T v) {
            inf = v;
            sup = v;
        }
        Interval() {
            inf = 0;
            sup = 0;
        }
        Interval(const Interval&p2) {
            inf = p2.inf;
            sup = p2.sup;
        }
        T getInf() const{return inf;}
        T getSup() const{return sup;}
        Interval<T> operator+(const Interval<T>&);
        Interval<T> operator-(const Interval<T>&);
        Interval<T> operator*(const Interval<T>&);
        Interval<T> operator/(const Interval<T>&);
};

template <class T>
T find_max(T a,T b){
    if(a < b){
        return b;
    }
    return a;
}

template <class T>
T find_min(T a,T b){
    if(a < b){
        return a;
    }
    return b;
}

template<class T>
Interval<T> Interval<T>::operator-(const Interval<T>&t){
    T start, end;
    start = inf - t.sup;
    end = sup - t.inf;
    Interval<T>m{start, end};
    return m;
}

template<class T>
Interval<T> Interval<T>::operator+(const Interval<T>&t){
    T start, end;
    start = inf + t.sup;
    end = sup + t.inf;
    Interval<T>m{start, end};
    return m;
}

template<class T>
Interval<T> Interval<T>::operator*(const Interval<T>&t){
    Interval<T> x;
    Interval<T> y;
    Interval<T> z;

    x.inf = inf * t.inf;
    x.sup = inf * t.sup;
    y.inf = sup * t.inf;
    y.sup = sup * t.sup;

    z.inf = find_min((find_min(x.inf,x.sup)),(find_min(y.inf,y.sup)));
    z.sup = find_max((find_max(x.inf,x.sup)),(find_max(y.inf,y.sup)));

    return z;
}

template<class T>
Interval<T>Interval<T>::operator/(const Interval<T>&t){
    if (t.inf == 0 || t.sup == 0){
      cerr << "ERROR: not able to divide by an interval containing zero." << endl;
      return 0;
    }
    else{
        Interval<T> x;
        Interval<T> y;
        Interval<T> z;

        x.inf = inf / t.inf;
        x.sup = inf / t.sup;
        y.inf = sup / t.inf;
        y.sup = sup / t.sup;

        z.inf = find_min((find_min(x.inf,x.sup)),(find_min(y.inf,y.sup)));
        z.sup = find_max((find_max(x.inf,x.sup)),(find_max(y.inf,y.sup)));
        return z;
    }
}

template <class T>
ostream& operator<<(ostream&os, const Interval<T>&t){
    os << "(" << t.getInf() << ", " << t.getSup() <<")"<<endl;;
    return os;
}

int main() {
    Interval p(5, 11);
    Interval q(1, 2);
    Interval r = p + q;
    Interval s = p - q;
    Interval w = p * q;
    Interval x = p / q;
    cout<<"p = "<< p;
    cout<<"q = "<< q;
    cout<<"p + q = "<< r;
    cout<<"p - q = "<< s;
    cout<<"p * q = "<< w;
    cout<<"p / q = "<< x;
}
