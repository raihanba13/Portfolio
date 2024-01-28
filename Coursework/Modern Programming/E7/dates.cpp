// class date, IO-operators
// author:  Holger Arndt
// version: 01.12.2020

// this does not work, I think I have made mistake in 81. Thanks for your concentration.

#include <iomanip>

#include <iostream>

using namespace std;

class Date {
    private:
        int day;
    int month;
    int year;
    static inline
    const int daysPerMonth[] {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };
    static
    const int stdYear {
        1970
    };
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }
    void checkDate();
    
    public:

        int dayInYear();

        explicit Date(int d = 1, int m = 1, int y = stdYear): day {
            d
        }, month {
            m
        },
        year {
            y
        } {
            checkDate();
        }
    Date(const Date & d2): day {
        d2.day
    }, month {
        d2.month
    }, year {
        d2.year
    } {}
    int getDay() const {
        return day;
    }
    int getMonth() const {
        return month;
    }
    int getYear() const {
        return year;
    }
    void setDate(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
        checkDate();
    }
    Date & incDay();
    Date & operator++();
    Date operator++(int);
    Date operator-();

    void print() const {
        cout << day << '.' << month << '.' << year << endl;
    }
};

int Date::dayInYear() {
    int i, dayOfYear = 0;
    for(i=1; i<month; i++){
        dayOfYear += daysPerMonth[i];
    }
    dayOfYear += day;
    return dayOfYear;
}

void Date::checkDate() {
    if (year < 1 || month < 1 || month > 12 || day < 1 ||
        (day > daysPerMonth[month - 1] &&
            !(day == 29 && month == 2 && isLeapYear(year)))) {
        day = month = 1;
        year = stdYear;
    }
}

Date & Date::incDay() {
    ++day;
    if (day > daysPerMonth[month - 1] &&
        !(day == 29 && month == 2 && isLeapYear(year))) {
        day = 1;
        if (++month == 13) {
            month = 1;
            ++year;
        }
    }
    return *this;
}

Date & Date::operator++() {
    incDay();
    return *this;
}

Date Date::operator++(int) {
    Date temp {
        * this
    };
    incDay();
    return temp;
}

ostream & operator << (ostream & os,
    const Date & d) {
    os << setw(2) << setfill('0') << d.getDay() << '.' <<
        setw(2) << d.getMonth() << '.' <<
        d.getYear() << setfill(' ');
    return os;
}

istream & operator >> (istream & is, Date & d) {
    int dd, mm, yy;
    is >> dd;
    is.ignore(1, '.');
    is >> mm;
    is.ignore(1, '.');
    is >> yy;
    d.setDate(dd, mm, yy);
    return is;
}

int Date::operator- (const Date& d2)
{
    int dY1, dY2; 
    dY1 = * this.dayInYear();
    dY2 = d2.dayInYear();

    return dY1 - dY2;
}

int main() {
    Date d;

    cout << "Enter a date: ";
    cin >> d;
    cout << d << endl;

    cout << "Date of the year (1): " << d.dayInYear() << endl; 

    Date d2;

    cout << "Enter a date: ";
    cin >> d2;
    cout << d2 << endl;

    cout << "Date of the year (2): " << d.dayInYear() << endl; 


    cout << "Difference between two date: " << d - d2 << endl; 
}