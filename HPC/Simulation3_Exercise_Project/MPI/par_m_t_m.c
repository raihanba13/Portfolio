// This has bug, read_distribute_matrix is not working

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

void read_distribute_matrix(FILE *file_pointer, double *M_local, int *row_local, int *row, int *col, int rank, int p){

    int i, j;
    MPI_Status status;

    if(rank == (p-1)){
        fscanf(file_pointer, "%d\n%d\n", row, col);
    }

    *row_local = *row / p;

    M_local = malloc((*row_local) * (*col) * sizeof(double));

    if(rank == (p-1)){
        for(i=0; i<p-1; i++){
            for(j=0; j<(*row_local)*(*col); j++){
                fscanf(file_pointer, "%lf\n", M_local+j);
            }
            // sending the data to the correct processor
            MPI_Send(M_local, (*row_local) * (*col), MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }

        // the last row block for p-1 processor
        for(j=0; j<(*row_local) * (*col); j++){
            fscanf(file_pointer, "%lf\n", M_local+j);
        }
    }else{
        MPI_Recv(M_local, (*row_local) * (*col), MPI_DOUBLE, p-1, rank, MPI_COMM_WORLD, &status);
    }
}

int main(int argc, char **argv){
    // all variable here (for all processor) ====================
    int rank, p, a_row, a_col, b_row, b_col, a_local_row, b_local_row;
    int i, j;

    double *a_local_array = NULL; 
    double *b_local_array = NULL; 
    double *c_local_array = NULL; 

    // all variable here ====================

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    // all input reading from p-1 processor
    // if(rank == (p-1)){
    FILE *file_pointer;
    file_pointer = fopen("matrix_t_matrix/test_a.txt", "r");
    read_distribute_matrix(file_pointer, a_local_array, &a_local_row, &a_row, &a_col, rank, p);
    fclose(file_pointer);

    FILE *file_pointer2;
    file_pointer2 = fopen("matrix_t_matrix/test_b.txt", "r");
    read_distribute_matrix(file_pointer2, b_local_array, &b_local_row, &b_row, &b_col, rank, p);
    fclose(file_pointer2);
    // }
    
    MPI_Bcast(&a_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&a_col, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&a_local_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);

    MPI_Bcast(&b_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&b_col, 1, MPI_INT, p-1, MPI_COMM_WORLD);
    MPI_Bcast(&b_local_row, 1, MPI_INT, p-1, MPI_COMM_WORLD);

    printf("rank = %d => a_row=%d, a_col=%d, a_local_row=%d\n", rank, a_row, a_col, a_local_row);
    printf("rank = %d => b_row=%d, b_col=%d, b_local_row=%d\n", rank, b_row, b_col, b_local_row);

    // char out_file_path[100];
    // sprintf(out_file_path, "data_%d.txt", rank);
    // FILE *out_file = fopen(out_file_path, "w+");
    // for(i=0; i<a_local_row*a_col; i++){
    //     fprintf(out_file, "%lf\n", a_local_array[i]);
    // }
    // fclose(out_file);

    MPI_Finalize();

    return 0;

}