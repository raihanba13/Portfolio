// this uses circular shift

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv){
    int rank, p, a_row, a_col, a_lrow, x_lrow; 
    int i, j, shift;
    int dest, source;

    double *a_l_array = NULL;
    double *x_l_array = NULL;
    double *b_l_array = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    // All I/o will be done from the last processor
    // read the size of A matrix
    if(rank == (p-1)){
        FILE *file_pointer;
        file_pointer = fopen("matrix_t_vector/size.txt", "r");
        fscanf(file_pointer, "%d %d", &a_row, &a_col);
        fclose(file_pointer);
    }

    MPI_Bcast(&a_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&a_col, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    
    a_lrow = a_row/p;
    x_lrow = a_col/p;
    // in matrix X vector, a_lrow and b_lrow will be same

    // printf("rank = %d => a_row=%d, a_col=%d, a_l_row=%d, x_l_row=%d\n", rank, a_row, a_col, a_lrow, x_lrow);

    a_l_array = malloc(a_lrow*a_col*sizeof(double));
    x_l_array = malloc(x_lrow*sizeof(double));
    b_l_array = calloc(a_lrow, sizeof(double));

    // now we read data a 
    if(rank == (p-1)){
        FILE *file_pointer2;
        file_pointer2 = fopen("matrix_t_vector/test_a.txt", "r");
        for(i=0; i<p-1; i++){
            for(j=0; j<a_lrow*a_col; j++){
                fscanf(file_pointer2, "%lf\n", a_l_array+j);
            }
            // sending the data to the correct processor
            MPI_Send(a_l_array, a_lrow*a_col, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }

        // the last row block for p-1 processor
        for(j=0; j<a_lrow*a_col; j++){
            fscanf(file_pointer2, "%lf\n", a_l_array+j);
            // a_col = a_col - 1 + 1;
        }

        fclose(file_pointer2);
    }else{
        MPI_Recv(a_l_array, a_lrow*a_col, MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD, &status);
    }

    // now we read data x
    if(rank == (p-1)){
        FILE *file_pointer2;
        file_pointer2 = fopen("matrix_t_vector/test_b.txt", "r");
        for(i=0; i<p-1; i++){
            for(j=0; j<x_lrow; j++){
                fscanf(file_pointer2, "%lf\n", x_l_array+j);
            }
            // sending the data to the correct processor
            MPI_Send(x_l_array, x_lrow, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }

        // the last row block for p-1 processor
        for(j=0; j<a_lrow*a_col; j++){
            fscanf(file_pointer2, "%lf\n", x_l_array+j);
        }

        fclose(file_pointer2);
    }else{
        MPI_Recv(x_l_array, x_lrow, MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD, &status);
    }

    // writing a, x to debug
    char out_file_path[100];
    sprintf(out_file_path, "data_%d.txt", rank);
    FILE *out_file = fopen(out_file_path, "w+");
    for(i=0; i<a_lrow*a_col; i++){
        fprintf(out_file, "%lf\n", a_l_array[i]);
    }
    fprintf(out_file, "\n===================\n");
    for(i=0; i<x_lrow; i++){
        fprintf(out_file, "%lf\n", x_l_array[i]);
    }
    fclose(out_file);

    // matrix X vector with circular shift
    for(shift=0; shift<p; shift++){
        for(i=0; i<a_lrow; i++){
            for(j=0; j<x_lrow; j++){
                b_l_array[i] += a_l_array[i*a_col+shift*x_lrow+j] * x_l_array[j];
            }
        }
        // TODO: improve send_tag and recv_tag
        dest = (rank - 1 + p) % p;
        source = (rank + 1) % p;
        // printf("rank %d => shift=%d, dest=%d, source=%d\n", rank, shift, dest, source);
        if(!rank){
            for(i=0; i<x_lrow; i++){
                printf("local x data: %lf\n", x_l_array[i]);
            }
        }
        MPI_Sendrecv_replace(
            x_l_array, x_lrow, MPI_DOUBLE, dest, 0, source, 0, MPI_COMM_WORLD, &status
        );

        if(!rank){
            printf("===============\n");
        }
    }

    // quick check, norm of b
    
    double norm = 0.0;
    if(rank == (p-1)){
        for(i=0; i<a_lrow; i++){
            norm += pow(b_l_array[i], 2);
        }
        for(i=0; i<p-1; i++){
            MPI_Recv(b_l_array, a_lrow, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
            for(j=0; j<a_lrow; j++){
                norm += pow(b_l_array[j], 2);
            }
        }
    }else{
        MPI_Send(b_l_array, a_lrow, MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD);
    }

    if(rank == (p-1)){
        printf("Norm of b: %lf", sqrt(norm));
    }
    

    MPI_Finalize();

    return 0;

}