#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

typedef struct GRID_INFO_T {
    int p;
    MPI_Comm comm;
    MPI_Comm row_comm;
    MPI_Comm col_comm;
    int q;
    int my_row;
    int my_col;
    int my_rank;
} GRID_INFO_T;

double* read_matrix2(
	char* filename,
	GRID_INFO_T* grid,
	int* m,
	int* n)
{
	int        matrix_row, matrix_col;
	int        grid_row, grid_col;
	int        dest;
	int        cart_coords[2];
	int        local_m, local_n;
	double*     loc_matrix_A;
	MPI_Status status;
	FILE *fp;

	if (grid->my_rank == 0)
	{
		fp = fopen(filename, "r");
		fscanf(fp, "%d", m);
		fscanf(fp, "%d", n);
	}

	MPI_Bcast(m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	local_m = *m / grid->q;
	local_n = *n / grid->q;

	loc_matrix_A = malloc(local_m*local_n*sizeof(double));

	if (grid->my_rank == 0)
	{
		double* temp = malloc(local_n*sizeof(double));
		for (matrix_row = 0; matrix_row < *m; matrix_row++)
		{
			grid_row = matrix_row / (local_m);
			cart_coords[0] = grid_row;
			for (grid_col = 0; grid_col < grid->q; grid_col++)
			{
				cart_coords[1] = grid_col;
				MPI_Cart_rank(grid->comm, cart_coords, &dest);
				if (dest == 0)
				{
					for (matrix_col = 0; matrix_col < local_n; matrix_col++)
					{
						fscanf(fp, "%f", &loc_matrix_A[matrix_row*local_n + matrix_col]);
					}
				}
				else {
					for (matrix_col = 0; matrix_col < local_n; matrix_col++)
					{
						fscanf(fp, "%f", &temp[matrix_col]);
					}
					MPI_Send(temp, local_n, MPI_FLOAT, dest, 0, grid->comm);
				}
			}
		}
		free(temp);
	}
	else
	{
		for (matrix_row = 0; matrix_row < local_m; matrix_row++)
		{
			MPI_Recv(&loc_matrix_A[matrix_row*local_n], local_n, MPI_FLOAT, 0, 0, grid->comm, &status);
		}
	}

	return loc_matrix_A;
}


double* read_matrix(char *filename, GRID_INFO_T *grid, int *m, int *n){
    int matrix_row, matrix_col;
    int grid_row, grid_col;
    int dest;
    int cart_coords[2];
    int local_m, local_n;
    double *local_matrix_A; 
    MPI_Status status; 
    FILE *fp;

    if(grid->my_rank == 0){
        fp = fopen(filename, "r");
        fscanf(fp, "%d", m);
        fscanf(fp, "%d", n);
    }

    MPI_Bcast(m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_m = *m / grid->q;
    local_n = *n / grid->q;

    // local_matrix_A = calloc(local_m*local_n, sizeof(double));
    local_matrix_A = malloc(local_m*local_n*sizeof(double));

    if(grid->my_rank == 0){
        double *temp = malloc(local_n*sizeof(double));

        for(matrix_row=0; matrix_row <*m; matrix_row++){
            grid_row = matrix_row / local_m;
            cart_coords[0] = grid_row;

            for(grid_col = 0; grid_col < grid->q; grid_col++){
                cart_coords[1] = grid_col;
                MPI_Cart_rank(grid->comm, cart_coords, &dest);

                if(dest == 0){
                    for(matrix_col = 0; matrix_col < local_n; matrix_col++){
                        fscanf(fp, "%lf", &local_matrix_A[matrix_row*local_n + matrix_col]);
                    }
                }else{
                    for(matrix_col = 0; matrix_col < local_n; matrix_col++){
                        fscanf(fp, "%lf", &temp[matrix_col]);
                    }
                    MPI_Send(temp, local_n, MPI_DOUBLE, dest, 0, grid->comm);
                }
            }
        }
        free(temp);
    }else{
        for(matrix_row = 0; matrix_row < local_m; matrix_row++){
            MPI_Recv(&local_matrix_A[matrix_row*local_n], local_n, MPI_DOUBLE, 0, 0, grid->comm, &status);
        }
    }

    return local_matrix_A;

}

void main(int argc, char *argv[]){
    double *local_A;
    double *local_B;
    double *local_C;
    int m, n, l;
    int local_m, local_n, local_l;
    double t_io_in, t_io_out, t_comp_in, t_comp_out;

    MPI_Init(&argc, &argv);

    struct GRID_INFO_T *grid = malloc(sizeof(struct GRID_INFO_T));

    // void Setup_grid(GRID_INFO_T* grid)
    int dimensions[2];
    int wrap_around[2];
    int coordinates[2];
    int free_coords[2];

    // p is number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->p));

    // We assume p is a perfect square
    grid->q = (int)sqrt ((float)grid->p);
    dimensions[0] = dimensions[1] = grid->q;

    // circular shift in second dimension
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->comm));
    MPI_Comm_rank(grid->comm, &(grid->my_rank));

    // get process coordinates in grid communicator
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    printf("Rank: %d => row=%d, col=%d\n", grid->my_rank, grid->my_row, grid->my_col);

    MPI_Barrier(MPI_COMM_WORLD);
    // set up row communicator 
    free_coords[0] = 0;
    free_coords[1] = 1;
    MPI_Cart_sub(grid->comm, free_coords, &(grid->row_comm));

    // set up column communicator
    free_coords[0] = 1;
    free_coords[1] = 0;
    MPI_Cart_sub(grid->comm, free_coords, &(grid->col_comm));

    MPI_Barrier(MPI_COMM_WORLD);
    t_io_in = MPI_Wtime();

    local_A = read_matrix("data/A.txt", grid, &m, &n);
    // local_A = read_matrix2("data/small_A.txt", grid, &m, &n);
    local_B = read_matrix("data/B.txt", grid, &n, &l);

    local_m = m / grid->q;
    local_n = n / grid->q;
    local_l = l / grid->q;

    char out_file_path[100];
    int i, j;
    sprintf(out_file_path, "data_%d.txt", grid->my_rank);
    FILE *out_file = fopen(out_file_path, "w+");
    for(i=0; i<local_m; i++){
        for(j=0; j<local_n; j++){
            fprintf(out_file, "%lf,", local_A[i*local_n+j]);
        }
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n===================\n");
    for(i=0; i<local_n; i++){
        for(j=0; j<local_l; j++){
            fprintf(out_file, "%lf,", local_B[i*local_l+j]);
        }
        fprintf(out_file, "\n");
    }
    // fprintf(out_file, "\n===================\n");
    // for(i=0; i<a_l_row*b_col; i++){
    //     fprintf(out_file, "%lf\n", c_l_array[i]);
    // }
    fclose(out_file);

    MPI_Finalize();

}