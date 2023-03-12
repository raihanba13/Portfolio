// this stores the whole x in every processor

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv){
    int rank, p, a_row, a_col, a_lrow; 
    int i, j;

    double *a_l_array = NULL;
    double *x_array = NULL;
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

    printf("rank = %d => row=%d, col=%d, l_row=%d\n", rank, a_row, a_col, a_lrow);

    a_l_array = malloc(a_lrow*a_col*sizeof(double));
    x_array = malloc(a_col*sizeof(double));
    b_l_array = malloc(a_lrow*sizeof(double));

    // now we read data a and x
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

    if(rank == (p-1)){
        FILE *file_pointer3;
        file_pointer3 = fopen("matrix_t_vector/test_b.txt", "r");
        for(j=0; j<a_col; j++){
            fscanf(file_pointer3, "%lf\n", x_array+j);
        }
    }

    MPI_Bcast(x_array, a_col, MPI_DOUBLE, p-1, MPI_COMM_WORLD);

    // multiply row with x and save it in b_local
    for(i=0; i<a_lrow; i++){
        b_l_array[i] = 0.0;
        for(j=0; j<a_col; j++){
            b_l_array[i] += a_l_array[j + i*a_col] * x_array[j];
        }
    }

    // writing b to debug
    char out_file_path[100];
    sprintf(out_file_path, "data_%d.txt", rank);
    FILE *out_file = fopen(out_file_path, "w+");
    for(i=0; i<a_lrow; i++){
        fprintf(out_file, "%lf\n", b_l_array[i]);
    }
    fclose(out_file);

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

}