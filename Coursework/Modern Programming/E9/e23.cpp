#include <iostream> 

using namespace std;

// Medium

class Medium {
    public: 
        int capacity;

        Medium();
        // Book(const Book& otherBook);
        // Book(std::string bookTitle);
        
        // std::string author, title, publisher, format;
        // int price; //given in cents
        // void SetYearOfPublication(int year);
        // int GetYearOfPublication() const; 
    // private:

    //     int mYearOfPublication;
};

Medium::Medium(){
    capacity = 0;
}

ostream & operator << (ostream & os, const Medium & m) {
  os << "Capacity of the medium: " <<  m.capacity << endl;
  return os;
}

// Harddisk

class HardDisk: public Medium {
    public: 
        int numberOfPartitions;

        HardDisk();
        HardDisk(int capacity, int numOfPar);
};

HardDisk::HardDisk(){
    numberOfPartitions = 1;
}

HardDisk::HardDisk(int capacity, int numOfPar){
    (*this).capacity = capacity;
    (*this).numberOfPartitions = numOfPar;
}

ostream & operator << (ostream & os, const HardDisk & h) {
  os << "Capacity of the HardDisk: " <<  h.capacity << endl;
  os << "Paritions of the HardDisk: " <<  h.numberOfPartitions << endl;
  return os;
}

// OpticalDisc

class OpticalDisc: public Medium {
    public: 
        string title;

        OpticalDisc();
        OpticalDisc(int capacity, string title);
};

OpticalDisc::OpticalDisc(){
    title = "unspecified";
}

OpticalDisc::OpticalDisc(int capacity, string title){
    (*this).capacity = capacity;
    (*this).title = title;
}

ostream & operator << (ostream & os, const OpticalDisc & o) {
  os << "Capacity of the OpticalDisc: " <<  o.capacity << endl;
  os << "Title of the OpticalDisc: " <<  o.title << endl;
  return os;
}

// CD

class CD: public OpticalDisc {
    public: 
        bool writable;

        CD();
        CD(int capacity, string title, bool writable);
};

CD::CD(){
    writable = false;
}

CD::CD(int capacity, string title, bool writable){
    (*this).capacity = capacity;
    (*this).title = title;
    (*this).writable = writable;
}

ostream & operator << (ostream & os, const CD & cd) {
  os << "Capacity of the CD: " <<  cd.capacity << endl;
  os << "Title of the CD: " <<  cd.title << endl;
  os << "CD can be writeable: " <<  cd.writable << endl;
  return os;
}

// DVD

 enum class format {
                type1, // DVD-ROM
                type2, // DVD+R(W)
                type3, // DVD-R(W)
                type4 // DVD-RAM
            };

class DVD: public OpticalDisc {
    public: 
        format f{};

        DVD();
        DVD(int capacity, string title, format f);
};

DVD::DVD(){
    f = format::type1;
}

DVD::DVD(int capacity, string title, format f){
    (*this).capacity = capacity;
    (*this).title = title;
    (*this).f = f;
}

ostream & operator << (ostream & os, const DVD & dvd) {
  os << "Capacity of the DVD: " <<  dvd.capacity << endl;
  os << "Title of the DVD: " <<  dvd.title << endl;

  if (dvd.f == format::type1){
      os << "Format of the DVD: DVD-ROM" << endl;
  }else if (dvd.f == format::type2){
      os << "Format of the DVD: DVD+R(W)" << endl;
  }else if (dvd.f == format::type3){
      os << "Format of the DVD: DVD-R(W)" << endl;
  }else{
      os << "Format of the DVD: DVD-RAM" << endl;
  }
  
  return os;
}


int main(){
    cout << "### Medium print ###" << endl;
    Medium m1;
    cout << m1;

    cout << "\n### HardDisk print (Default Constructor) ###" << endl;
    HardDisk h1;
    cout << h1;

    cout << "\n### HardDisk print (Custom Constructor with 2 input) ###" << endl;
    HardDisk h2(10000, 5); 
    cout << h2;

    cout << "\n### OpticalDisc print (Default Constructor) ###" << endl;
    OpticalDisc o1;
    cout << o1;

    cout << "\n### OpticalDisc print (Custom Constructor with 2 input) ###" << endl;
    OpticalDisc o2(500, "Linkin Park Vol1");
    cout << o2;

    cout << "\n### CD print (Default Constructor) ###" << endl;
    CD cd1;
    cout << cd1;

    cout << "\n### CD print (Custom Constructor with 3 input) ###" << endl;
    CD cd2(500, "Linkin Park Vol1", true);
    cout << cd2;

    cout << "\n### DVD print (Default Constructor) ###" << endl;
    DVD dvd1;
    cout << dvd1;

    cout << "\n### DVD print (initialize with capacity, title and format) ###" << endl;
    DVD dvd2(500, "Class Party 2021", format::type1);
    cout << dvd2;

    cout << "\n### DVD print (initialize with capacity, title and format) ###" << endl;
    DVD dvd3(1500, "Class Party 2021", format::type3);
    cout << dvd3;


}

