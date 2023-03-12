#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void trap(double a, double b, double *result){
    int n = omp_get_num_threads();
    int m = omp_get_thread_num();
    double h = (b-a)/n;
    double x = a + m*h;

    #pragma omp critical
    *result += h*x*x;
}

int main(int argc, char **argv){
    int thread_count = strtol(argv[1], NULL, 10);
    double a = 1;
    double b = 4;
    double result = 0;
    double tin, tout;

    tin = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count)
        trap(a, b, &result);

    tout = omp_get_wtime();
        
    printf("With %d trapezoids the estimate of the integral of x^2 between %f and %f is %f\n", thread_count, a, b, result);
    printf("Required time: %lf\n", tout-tin);
    return 0;
}