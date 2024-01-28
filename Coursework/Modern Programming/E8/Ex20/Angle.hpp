#ifndef ANGLE_H
#define ANGLE_H 

class Angle {
    private:
        int degrees;
        int minutes;
        double seconds;

        void destroy();

    public:
        Angle(): 
            degrees{0},
            minutes{0},
            seconds{0.0} {}

        void setDegree(int d);
        void setMinute(int m);
        void setSecond(double s);

        int getDegree() const;
        int getMinute() const;
        double getSecond() const;
        ~Angle() { destroy(); };

        void setAngle(int d, int m, double s);

        double GetAsDegrees();
        double toRadian();
        Angle& normalize(int dd, int mm, double ss);

        Angle operator+(const Angle& otherAngle);
        Angle operator-(const Angle& otherAngle);
};

std::ostream & operator << (std::ostream & os, const Angle & a);
std::istream & operator >> (std::istream & is, Angle & a);

#endif