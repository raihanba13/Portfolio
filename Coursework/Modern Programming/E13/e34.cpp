#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Dish {
    protected:
        double price;
        string description;
    public:
        Dish() : price{0.0}, description{"Undefined"} {}
        Dish(double price, string description) : price{price}, description{description} {}

        virtual string getDescription();
        virtual double getPrice();

        friend ostream & operator << (ostream & os, const Dish & d);

};

string Dish::getDescription(){
    return description;
}

double Dish::getPrice(){
    return price;
}

ostream & operator << (ostream & os, const Dish & d) {
  os << "Description: " <<  d.description << endl;
  os << "Price: " <<  d.price << endl;
  return os;
}

class VienneseSchnitzel: public Dish {
    public:
        VienneseSchnitzel() : Dish() {}
        VienneseSchnitzel(double price, string description) : Dish(price, description) {}
};

class Accompaniment: public Dish {
    public:
        shared_ptr <Dish> dish;
        Accompaniment(shared_ptr<Dish> d) : Dish(), dish(move(d)) {}
        Accompaniment(shared_ptr<Dish> d, double price, string description) : Dish(price, description), dish(move(d)) {}

        string getDescription() override {
            // cout << this->dish->getDescription();
            return this->dish->getDescription() + " with " + description;
        }

        double getPrice() override {
            return this->dish->getPrice() + price;
        }

};

class Chips: public Accompaniment {
    public:
        Chips(shared_ptr<Dish> d) : Accompaniment(d) {}
        Chips(shared_ptr<Dish> d, double price, string description) : Accompaniment(d, price, description) {}
};

class Salad: public Accompaniment {
    public:
        Salad(shared_ptr<Dish> d) : Accompaniment(d) {}
        Salad(shared_ptr<Dish> d, double price, string description) : Accompaniment(d, price, description) {}
};

int main(){
    Dish d1(5.0, "Doner Tasche");
    cout << d1;

    VienneseSchnitzel d2(7.0, "Friday Special Chicken VienneseSchnitzel");
    cout << d2;

    // testing
    shared_ptr <Dish> d3{ new Dish(50, "Asian Plain Rice")};

    Salad a1(d3, 2.5, "Mixed Veg & Chicken Salad");
    // << operator is overloaded to show the value of description & price
    cout << a1;
    // using Decorator pattern & method overloading we can see the updated value through getter methods
    cout << "Description: " << a1.getDescription() << endl;
    cout << "Price: " << a1.getPrice() << endl;
}
