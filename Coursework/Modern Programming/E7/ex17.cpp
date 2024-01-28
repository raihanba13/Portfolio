#include <iostream>
using namespace std;

class Time {
    private:
        // Private attribute
        int minute, hour;

    public:
        Time() {
            minute = 1;
            hour = 1;
        }

        Time(int m, int h) {
            int mFlag = setMinute(m);
            int hFlag = setHour(h);
        }

        // Setter
        int setMinute(int m) {
            if (m >= 0 && m <= 60){
                minute = m;
                return true;
            }else{
                return false;
            }
            
        }
        // Getter
        int getMinute() {
            return minute;
        }

        // Setter
        int setHour(int h) {
            if (h >= 0 && h <= 23){
                hour = h;
                return true;
            }else{
                return false;
            }
        }
        // Getter
        int getHour() {
            return hour;
        }
};

int main() {
    Time tObj(10, 10);

    // if the obj is created like this, the initial value will be 1, 1
    // Time tObj();

    cout << "Minute set at initialization: " << tObj.getMinute() << endl;
    cout << "Hour set at initialization: " << tObj.getHour() << endl;
    
    bool mSetFlag = tObj.setMinute(2);
    if (mSetFlag){
        cout << "Minute: " << tObj.getMinute() << endl;
    }else{
        cout << "The input argument is not right. Default Minute: " << tObj.getMinute() << endl;
    }

    bool hSetFlag = tObj.setHour(20);
    if (hSetFlag){
        cout << "Hour: " << tObj.getHour() << endl;
    }else{
        cout << "The input argument is not right. Default Hour: " << tObj.getHour() << endl;
    }
    
    return 0;
}