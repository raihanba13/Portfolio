#ifndef DATE_H
#define DATE_H

namespace mydate {
    class Date
    {
    private:
      int day;
      int month;
      int year;
      static inline const int daysPerMonth[]{ 31, 28, 31, 30, 31, 30, 31, 31, 30,
          31, 30, 31 };
      static const int stdYear{1970};
      static bool isLeapYear(int year)
      { return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }
      void checkDate();
    public:

      explicit Date(int d = 1, int m = 1, int y = stdYear) : day{d}, month{m},
                                                             year{y}
      {
        checkDate();
      }
      Date(const Date& d2) : day{d2.day}, month{d2.month}, year{d2.year} {}
      int getDay() const { return day; }
      int getMonth() const { return month; }
      int getYear() const { return year; }
      void setDate(int d, int m, int y)
      {
        day = d;
        month = m;
        year = y;
        checkDate();
      }
      Date& incDay();
      Date& operator++();
      Date operator++(int);
      void print() const { std::cout << day << '.' << month << '.' << year << std::endl; }

    };
}


std::ostream& operator<<(std::ostream& os, const mydate::Date& d);
std::istream& operator>>(std::istream& is, mydate::Date& d);

#endif
