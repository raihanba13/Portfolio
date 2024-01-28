#include <iostream>
#include <cstdarg>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "Angle.hpp"

using namespace std;

int Angle::getDegree() const {
    return degrees;
}
int Angle::getMinute() const {
    return minutes;
}
double Angle::getSecond() const {
    return seconds;
}

void Angle::setDegree(int d) {
    degrees = d;
}
void Angle::setMinute(int m) {
    minutes = m;
}
void Angle::setSecond(double s) {
    seconds = s;
}

void Angle::destroy() {
    // tried to write a Destructor
    // my understanding is delete and delete[] for new and new[] respectively
    // free for maloc
    // my question is, do I need to free primitive variable, is there any way for that?
    // Or it is automatically handled
    // free(degrees);
    // free(minutes);
    // free(seconds);
}

void Angle::setAngle(int d, int m, double s){
    assert(d >= 0 && d <= 359);
    assert(m >= 0 && m <= 59);
    assert(s >= 0.0 && s < 60.0);

    setDegree(d);
    setMinute(m);
    setSecond(s);
}

double Angle::GetAsDegrees(){
    double degFrac = 0.0;

    degFrac += (*this).degrees;
    degFrac += (*this).minutes/60.0;
    degFrac += (*this).seconds/3600.0;

    return degFrac;
}

Angle Angle::operator+(const Angle& otherAngle){
    int sDeg, sMin;
    double sSec;

    sDeg = (*this).getDegree() + otherAngle.getDegree();
    sMin = (*this).getMinute() + otherAngle.getMinute();
    sSec = (*this).getSecond() + otherAngle.getSecond();

    Angle sA;
    // in case the sum breaks our range
    sA.normalize(sDeg, sMin, sSec);

    return sA;

}

double Angle::toRadian(){
    return (*this).GetAsDegrees()*(3.1416/180.0);
}

Angle& Angle::normalize(int dd, int mm, double ss){

    double new_s = fmod(ss, 60.0); 
    int add_m = ss / 60; 
    mm = mm + add_m;
    int new_m = mm % 60;
    dd = (dd + (mm / 60)) % 360 ;

    (*this).setAngle(dd, new_m, new_s);

    return *this;
}

ostream & operator << (ostream & os, const Angle & a) {
    os << a.getDegree() << " d " << a.getMinute() << " m " << a.getSecond() << " s " << endl;
    return os;
}

istream & operator >> (istream & is, Angle & a) {
    int dd, mm;
    double ss;
    is >> dd;
    is.ignore(1, 'd');
    is >> mm;
    is.ignore(1, 'm');
    is >> ss;
    is.ignore(1, 's');
    a.setAngle(dd, mm, ss);
    return is;
}

