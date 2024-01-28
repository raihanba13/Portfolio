// class date, IO-operators
// author:  Holger Arndt
// version: 01.12.2020

#include <iomanip>
#include <iostream>
#include <exception>
#include <string>

using namespace std;

class DateException
{
public:
  enum class DateExcType { invalidDay, invalidMonth, invalidYear, invalidLeapYear, unknown };
private:
  DateExcType type;
  std::string where;
public:
  DateException() : type{DateExcType::unknown} {}
  DateException(DateExcType det) : type{det} {}
  DateException(DateExcType det, const std::string& w) : type{det}, where{w} {}
  std::string what() const;
  friend class Date;
};

string DateException::what() const
{
    string msg{"Error in class Date: "};
    switch (type)
        {
        case DateExcType::invalidDay:
            msg += "Invalid Day.";
            break;
        case DateExcType::invalidMonth:
            msg += "Invalid Month.";
            break;
        case DateExcType::invalidYear:
            msg += "Invalid Year.";
            break;
        case DateExcType::invalidLeapYear:
            msg += "Invalid Year.";
            break;
        default: //unknown
            msg += "unknown error";
            break;
        }
    if (!where.empty())
        msg += (" in " + where);

    return msg;
}

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
  ~Date() {}
  explicit Date(int d = 1, int m = 1, int y = stdYear) : day{d}, month{m},
                                                         year{y}
  {
      cout << "Checking constructor... " << endl;
      try{
          checkDate();
          cout << "constructor worked" << endl;
      } catch (DateException& de){
          cerr << de.what() << endl;
          // ~Date();
      }
  }
  Date(const Date& d2) : day{d2.day}, month{d2.month}, year{d2.year} {}
  int getDay() const { return day; }
  int getMonth() const { return month; }
  int getYear() const { return year; }
  void setDate(int d, int m, int y)
  {

    cout << "Checking setDate... " << endl;
    try{

        day = d;
        month = m;
        year = y;
        checkDate();
        cout << "setDate worked" << endl;
        // this does not work from here
        // cout << *this << endl;
    } catch (DateException& de){
        cerr << de.what() << endl;
        // I checked the conditions but could not stop the object being created
        // ~Date();
    }

  }
  Date& incDay();
  Date& operator++();
  Date operator++(int);
  void print() const { cout << day << '.' << month << '.' << year << endl; }
  friend class DateException;
};

void Date::checkDate()
{
    // error handling
    // date exceds the range against month
    if((day > daysPerMonth[month - 1]) || day < 1){
        throw DateException{DateException::DateExcType::invalidDay, std::to_string(day)};
    } else if(day == 29 && !(month == 2 && isLeapYear(year))){
        throw DateException{DateException::DateExcType::invalidLeapYear, std::to_string(year)};
    } else if(month > 12 || month < 1){
        throw DateException{DateException::DateExcType::invalidMonth, std::to_string(month)};
    } else if(year < 1){
        throw DateException{DateException::DateExcType::invalidYear, std::to_string(year)};
    }


  if (year < 1 || month < 1 || month > 12 || day < 1
      || (day > daysPerMonth[month - 1]
          && !(day == 29 && month == 2 && isLeapYear(year))))
    {
      day = month = 1;
      year = stdYear;
    }
}

Date& Date::incDay()
{
  ++day;
  if (day > daysPerMonth[month - 1]
      && !(day == 29 && month == 2 && isLeapYear(year)))
    {
      day = 1;
      if (++month == 13)
        {
          month = 1;
          ++year;
        }
    }
  return *this;
}

Date& Date::operator++()
{
  incDay();
  return *this;
}

Date Date::operator++(int)
{
  Date temp{*this};
  incDay();
  return temp;
}

ostream& operator<<(ostream& os, const Date& d)
{
  os << setw(2) << setfill('0') << d.getDay() << '.'
     << setw(2) << d.getMonth() << '.'
     << d.getYear() << setfill(' ');
  return os;
}

istream& operator>>(istream& is, Date& d)
{
  int dd, mm, yy;
  is >> dd;
  is.ignore(1, '.');
  is >> mm;
  is.ignore(1, '.');
  is >> yy;
  d.setDate(dd, mm, yy);
  return is;
}

int main()
{
  Date d;

  cout << "Enter a date: ";
  cin >> d;
}
