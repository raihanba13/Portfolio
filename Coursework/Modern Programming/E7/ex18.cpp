#include <iostream>
using namespace std;

class Car {
    private:
        // Private attribute
        string  type;
        double mileage;

    public:
        Car() {
            mileage = 0.0;
            type = "";
        }

        // Setter
        int setType(string t) {
            type = t;
        }
        // Getter
        string getType() {
            return type;
        }

        // Setter
        int setMileage(double m) {
            mileage = m;
        }
        // Getter
        int getMileage() {
            return mileage;
        }

        int drive(double m) {
            double newM = mileage + m;
            setMileage(newM);
            return newM;
        }

        void print(){
            cout << "Type: " << type << endl;
            cout << "Current Mileage: " << mileage << endl;
        }

        int tow(Car &cObj2, double m){
            setMileage(getMileage() + m);
            cObj2.setMileage(cObj2.getMileage() + m);
        }
};

int main() {

    Car cObj1;

    cObj1.setType("Sedan");
    cout << "\n\n======Car1======\n\n";
    cObj1.print();
    cout << "\n\n============\n\n";

    cout << "Mileage at beginning (Car 1): " << cObj1.getMileage() << endl;
    // drive the car for 5 unit
    double currentM = cObj1.drive(18.0);
    cout << "Mileage After 18 unit driving (Car 1): " << cObj1.getMileage() << endl;

    currentM = cObj1.drive(13.0);
    cout << "Mileage After 18+13 unit driving (Car 1): " << cObj1.getMileage() << endl;

    // lets create another car obj    
    Car cObj2;

    cObj2.setType("Pick up");
    cout << "\n\n======Car2======\n\n";
    cObj2.print();
    cout << "\n\n============\n\n";
    
    cout << "Mileage at beginning (Car 2): " << cObj2.getMileage() << endl;
    // drive the car for 5 unit
    double currMileage2 = cObj2.drive(7.0);
    cout << "Mileage After 7 unit driving (Car 2): " << cObj2.getMileage() << endl;

    currMileage2 = cObj2.drive(9.0);
    cout << "Mileage After 7+9 unit driving (Car 2): " << cObj2.getMileage() << endl;

    // tow and drive for 7 unit
    cObj1.tow(cObj2, 23.00);
    cout << "\n\nMileage After 23 unit tow (Car 1): " << cObj1.getMileage() << endl;
    cout << "Mileage After 23 unit driving (Car 2): " << cObj2.getMileage() << endl;
    
    return 0;
}