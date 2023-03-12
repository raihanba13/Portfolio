#include <stdio.h>

double func(double x){
    // return x*x;
    return 1.0/(1 + x*x);
}

int main(){
    double integral;
    double a, b;
    int n; //number of trapezoidals
    double h; //base width
    double x;
    int i;

    printf("Enter a, b and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    h = (b-a)/n;

    integral = 0.5 * (func(a) + func(b));

    x = a;
    for(i=1; i<=n-1; i++){
        x += h;
        integral += func(x);
    }
    integral *= h; 

    printf("With %d trapezoids, upper limit a=%lf to lower limit b=%lf, our estimation is %lf", n, a, b, integral);

    return 0;
}