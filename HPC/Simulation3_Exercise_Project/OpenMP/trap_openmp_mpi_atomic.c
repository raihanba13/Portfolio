#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

void trap(double a, double b, double *result, int rank, int thread){
    int n = omp_get_num_threads();
    int m = omp_get_thread_num();
    double h = (b-a)/n;
    double x = a + m*h;

    printf("rank = %d => thread = %d\n", rank, thread);

    #pragma omp atomic
    *result += h*x*x;
}

int main(int argc, char **argv){
    // int thread_count = strtol(argv[1], NULL, 10);
    int thread_count = 500;
    double a = 1;
    double b = 4;
    double result = 0;
    double tin, tout;

    omp_set_num_threads(thread_count);

    int p, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double local_a, local_b;
    local_a = a + rank * (b-a)/p;
    local_b = a + (rank+1) * (b-a)/p;

    // tin = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count)
        trap(local_a, local_b, &result, rank, omp_get_thread_num());

    // tout = omp_get_wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    double global_result = 0.0;
    MPI_Reduce(&result, &global_result, sizeof(double), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        
    if(!rank){
        printf("With %d trapezoids the estimate of the integral of x^2 between %f and %f is %f\n", thread_count, a, b, global_result);
        printf("Required time: %lf\n", 0);
    }
    

    MPI_Finalize();
    return 0;
}