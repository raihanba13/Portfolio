// This uses Allreduce with pointer to a function

#include <stdio.h>
#include <math.h>
#include "mpi.h"

double func(double x){
    // return x*x;
    return 1.0/(1 + x*x);
}

void local_T2N_calc(double local_a, int local_n, double h, double *local_T2N, double (*f_ptr)(double) ){
    int i;
    double step;
    // local_T2N is passed by reference
    step = local_a + h;
    for(i=0; i<=local_n-1; i+=2){
        *local_T2N += (*f_ptr)(step);
        step += 2*h;
    }

    *local_T2N *= h;

}

int main(int argc, char* argv[]){

    // MPI variable
    int rank;
    int p;
    int source, dest;
    int tag=0;
    char message[100];
    MPI_Status status;

    // start the MPI world
    MPI_Init(&argc, &argv);
    // my rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Find out number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double integral;
    double a, b;
    int n; //number of trapezoidals
    double h; //base width
    double x;
    int i;

    double (*f_ptr)(double) = &func;

    a = 0.0, b = 1.0, integral = 0.0;
    
    double local_a, local_b;
    double local_integral = 0.0;
    double T2N = 10.0, local_T2N, TN = 0.0, local_TN, S2N = 20.0, tol = 10e-8;
    int local_n = 2;
    int iter_count = 0;
    n = 2 * p;
    h = (b - a)/n;

    local_a = a + rank * local_n * h;
    local_b = a + (rank+1) * local_n * h;

    local_TN = (((*f_ptr)(local_a) + (*f_ptr)(local_b)) / 2.0 + (*f_ptr)(local_a+h)) * h; 
    MPI_Allreduce(&local_TN, &TN, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    // MPI_Reduce(&local_TN, &TN, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&TN, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    while(fabs(S2N-T2N) >= tol){
        h /= 2.0;
        local_n *= 2;

        // we calculate local_T2N
        local_T2N_calc(local_a, local_n, h, &local_T2N, (*f_ptr));
        MPI_Allreduce(&local_T2N, &T2N, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        T2N += TN * 0.5; 
        S2N = (4/3.0)*T2N - (1/3.0)*TN;

        TN = T2N;

        // keeping the result form root 0 to debug
        if(!rank){
            printf("Iter No: %d\n", iter_count++);
            printf("Calculated PI: %lf\n", 4 * S2N);
            printf("Exec PI: %lf\n", M_PI);
            printf("fabs(S2N-T2N): %lf\n", fabs(S2N-T2N));
            printf("========================================\n");
        }
    }

    if(!rank){
        printf("Final Result ########\n");
        printf("Iter No: %d\n", iter_count);
        printf("Calculated PI: %lf\n", 4 * S2N);
        printf("Exec PI: %lf\n", M_PI);
        printf("========================================");
    }

    // writing the data to debug
    char out_file_path[100];
    sprintf(out_file_path, "data_%d.txt", rank);
    FILE *out_file = fopen(out_file_path, "w+");
    fprintf(out_file, "%lf\n", local_a);
    fprintf(out_file, "%lf\n", local_b);

    fclose(out_file);

    MPI_Finalize();

    return 0;
}