#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv){
    // all variable here (for all processor) ====================
    int rank, p, a_row, a_col, b_row, b_col, a_l_row, b_l_row;
    int i, j, k, shift;
    int dest, source;

    double *a_l_array = NULL; 
    double *b_l_array = NULL; 
    double *c_l_array = NULL; 

    FILE *file_pointer, *file_pointer2;

    // all variable here ====================

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    // all input reading from p-1 processor
    // reading the size of A and B
    if(rank == (p-1)){
        file_pointer = fopen("matrix_t_matrix/test_a.txt", "r");
        fscanf(file_pointer, "%d\n%d\n", &a_row, &a_col);
        a_l_row = a_row / p;

        file_pointer2 = fopen("matrix_t_matrix/test_b.txt", "r");
        fscanf(file_pointer2, "%d\n%d\n", &b_row, &b_col);
        b_l_row = b_row / p;
    }
    
    // Bcast of size of A, B and allocate memory for all local matrix
    MPI_Bcast(&a_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&a_col, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&a_l_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);

    MPI_Bcast(&b_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&b_col, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&b_l_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);

    a_l_array = malloc(a_l_row*a_col*sizeof(double));
    b_l_array = malloc(b_l_row*b_col*sizeof(double));
    c_l_array = calloc(a_l_row*b_col, sizeof(double));

    printf("rank = %d => a_row=%d, a_col=%d, a_local_row=%d\n", rank, a_row, a_col, a_l_row);
    printf("rank = %d => b_row=%d, b_col=%d, b_local_row=%d\n", rank, b_row, b_col, b_l_row);

    // read and distribute A, B 
    if(rank == (p-1)){
        for(i=0; i<p-1; i++){
            for(j=0; j<a_l_row*a_col; j++){
                fscanf(file_pointer, "%lf\n", a_l_array+j);
            }
            MPI_Send(a_l_array, a_l_row*a_col, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }
        // last block for p-1 processor
        for(j=0; j<a_l_row*a_col; j++){
            fscanf(file_pointer, "%lf\n", a_l_array+j);
        }

        for(i=0; i<p-1; i++){
            for(j=0; j<b_l_row*b_col; j++){
                fscanf(file_pointer2, "%lf\n", b_l_array+j);
            }
            MPI_Send(b_l_array, b_l_row*b_col, MPI_DOUBLE, i, i*(-1), MPI_COMM_WORLD);
        }

        // last block for p-1 processor
        for(j=0; j<b_l_row*b_col; j++){
            fscanf(file_pointer2, "%lf\n", b_l_array+j);
        }

        fclose(file_pointer);
        fclose(file_pointer2);
    }else{
        MPI_Recv(a_l_array, a_l_row*a_col, MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_l_array, b_l_row*b_col, MPI_DOUBLE, p-1, rank*(-1), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // matrix matrix multiplication with circular shift
    for(shift=0; shift<p; shift++){
        for(i=0; i<a_l_row; i++){
            for(j=0; j<b_col; j++){
                for(k=0; k<b_l_row; k++){
                    c_l_array[i*b_col+j] += a_l_array[i*a_col+((shift+rank)%p)*b_l_row+k] * b_l_array[k*b_col+j];
                    // using only shift is not correct, below one is wrong
                    // c_l_array[i*b_col+j] += a_l_array[i*a_col+shift*b_l_row+k] * b_l_array[k*b_col+j];
                }
            }
        }

        dest = (rank - 1 + p) % p;
        source = (rank + 1) % p;
        MPI_Sendrecv_replace(b_l_array, b_l_row*b_col, MPI_DOUBLE, dest, 0, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    char out_file_path[100];
    sprintf(out_file_path, "data_%d.txt", rank);
    FILE *out_file = fopen(out_file_path, "w+");
    for(i=0; i<a_l_row; i++){
        for(j=0; j<a_col; j++){
            fprintf(out_file, "%lf,", a_l_array[i*a_col+j]);
        }
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n===================\n");
    for(i=0; i<b_l_row; i++){
        for(j=0; j<b_col; j++){
            fprintf(out_file, "%lf,", b_l_array[i*b_col+j]);
        }
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n===================\n");
    for(i=0; i<a_l_row*b_col; i++){
        fprintf(out_file, "%lf\n", c_l_array[i]);
    }
    fclose(out_file);

    // quick check, norm of b
    
    double norm = 0.0;
    if(rank == (p-1)){
        for(i=0; i<a_l_row*b_col; i++){
            norm += pow(c_l_array[i], 2);
        }
        for(i=0; i<p-1; i++){
            MPI_Recv(c_l_array, a_l_row*b_col, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
            for(j=0; j<a_l_row*b_col; j++){
                norm += pow(c_l_array[j], 2);
            }
        }
    }else{
        MPI_Send(c_l_array, a_l_row*b_col, MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD);
    }

    if(rank == (p-1)){
        printf("Norm of b: %lf", sqrt(norm));
    }

    MPI_Finalize();

    return 0;

}