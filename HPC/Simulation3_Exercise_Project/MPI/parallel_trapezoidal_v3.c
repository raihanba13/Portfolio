// This uses Send and Recv, and also timing

#include <stdio.h>
#include <math.h>
#include "mpi.h"

double func(double x){
    // return x*x;
    return 1.0/(1 + x*x);
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

    double t_io_in, t_io_out, t_comp_out;


    t_io_in = MPI_Wtime();
    if(!rank){
        FILE *in_file = fopen("input.txt", "r");
        printf("P0: Reading a, b and n from input.txt\n");
        fscanf(in_file, "%lf %lf %d", &a, &b, &n);
        printf("P0: a=%lf, b=%lf and n=%d.\n", a, b, n);

        h = (b-a)/n;

        fclose(in_file);

        for(i=1; i<p; i++){
            tag = 0;
            MPI_Send(&h, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            tag = 1;
            MPI_Send(&a, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            tag = 2;
            MPI_Send(&b, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            tag = 3;
            MPI_Send(&n, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
    }else{
        tag = 0;
        MPI_Recv(&h, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        tag = 1;
        MPI_Recv(&a, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        tag = 2;
        MPI_Recv(&b, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        tag = 3;
        MPI_Recv(&n, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
    }

    // must be executed by all processor
    // MPI_Bcast(&h, sizeof(double), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&a, sizeof(double), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&b, sizeof(double), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    t_io_out = MPI_Wtime();
    
    double local_a, local_b;
    int local_n;
    double local_integral = 0.0;

    local_a = a + ((double) rank*(b-a))/(double) p;
    local_b = a + ((rank+1)*(b-a))/(double) p;
    local_n = n/p;

    // writing the data to debug
    char out_file_path[100];
    sprintf(out_file_path, "data_%d.txt", rank);
    FILE *out_file = fopen(out_file_path, "w+");
    fprintf(out_file, "%lf\n", local_a);
    fprintf(out_file, "%lf\n", local_b);
    fprintf(out_file, "%lf\n", n);
    fprintf(out_file, "%d\n", local_n);
    fprintf(out_file, "%lf\n", h);
    fprintf(out_file, "%d\n", p);
    fprintf(out_file, "%lf\n", b-a);
    fprintf(out_file, "%lf\n", a);
    fprintf(out_file, "%lf\n", b);

    // we add the first and last limit for all cases
    if(!rank){
        local_integral = 0.5 * func(local_a) + func(local_b);
    }else if(rank == p-1){
        local_integral = func(local_a) + 0.5 * func(local_b);
    }else{
        local_integral = func(local_a) + func(local_b);
    }

    // now we calculate the rest, clearly we have now local_n-2 trapezoids for each processor
    for(i=1; i<=local_n-1; i++){
        local_a += h;
        local_integral += func(local_a);
    }
    local_integral *= h; 

    
    fclose(out_file);

    if(rank){
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
    }
    

    MPI_Barrier(MPI_COMM_WORLD);

    t_comp_out = MPI_Wtime();

    if(!rank){
        integral = local_integral;
        for(i=1; i<p; i++){
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &status);
            integral += local_integral;
        }

        printf("With %d trapezoids, upper limit a=%lf to lower limit b=%lf, our estimation of pi is %lf\n", n, a, b, integral*4);
        printf("True value of Pi is %lf\n", M_PI);
        printf("I/O Time %lf\n", t_io_out - t_io_in);
        printf("Computational Time %lf\n", t_comp_out - t_io_out);
    }

    MPI_Finalize();

    return 0;
}