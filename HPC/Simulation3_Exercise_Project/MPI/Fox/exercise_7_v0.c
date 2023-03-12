#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

typedef struct GRID_INFO_T
{
	int p;
	MPI_Comm comm;
	MPI_Comm row_comm;
	MPI_Comm col_comm;
	int q;
	int my_row;
	int my_col;
	int my_rank;
}GRID_INFO_T;

float* read_matrix(
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
	float*     loc_matrix_A;
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

	loc_matrix_A = malloc(local_m*local_n*sizeof(float));

	if (grid->my_rank == 0)
	{
		float* temp = malloc(local_n*sizeof(float));
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

void Setup_grid(GRID_INFO_T* grid)
{
	int dimensions[2];
	int wrap_around[2];
	int coordinates[2];
	int free_coords[2];

	/* set up global grid information */
	MPI_Comm_size(MPI_COMM_WORLD, &(grid->p));

	/* We assume p is a perfect square */
	grid->q = (int)sqrt((float)grid->p);
	dimensions[0] = dimensions[1] = grid->q;

	/* circular shift in second dimension */
	wrap_around[0] = wrap_around[1] = 1;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->comm));
	MPI_Comm_rank(grid->comm, &(grid->my_rank));

	/* get process coordinates in grid communicator */
	MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
	grid->my_row = coordinates[0];
	grid->my_col = coordinates[1];

	MPI_Barrier(MPI_COMM_WORLD);
	/* set up row communicator */
	free_coords[0] = 0;
	free_coords[1] = 1;
	MPI_Cart_sub(grid->comm, free_coords, &(grid->row_comm));

	/* set up column communicator */
	free_coords[0] = 1;
	free_coords[1] = 0;
	MPI_Cart_sub(grid->comm, free_coords, &(grid->col_comm));
}



void print_matrix(
	char* filename,
	GRID_INFO_T* grid,
	float* loc_matrix_A,
	int m, int n)
{
	int        matrix_row, matrix_col;
	int        grid_row, grid_col;
	int        source;
	int        cart_coords[2];
	float*     temp;
	MPI_Status status;
	FILE *fp;

	int local_m = m / grid->q;
	int local_n = n / grid->q;

	if (grid->my_rank == 0)
	{
		fp = fopen(filename, "w");
		fprintf(fp, "%d\n", m);
		fprintf(fp, "%d\n", n);
		temp = malloc(local_n*sizeof(float));
		for (matrix_row = 0; matrix_row < m; matrix_row++)
		{
			grid_row = matrix_row / local_m;
			cart_coords[0] = grid_row;
			for (grid_col = 0; grid_col < grid->q; grid_col++)
			{
				cart_coords[1] = grid_col;
				MPI_Cart_rank(grid->comm, cart_coords, &source);
				if (source == 0)
				{
					for (matrix_col = 0; matrix_col < local_n; matrix_col++)
					{
						fprintf(fp, "%f\n", loc_matrix_A[matrix_row*local_n + matrix_col]);
					}
				}
				else
				{
					MPI_Recv(temp, local_n, MPI_FLOAT, source, 0, grid->comm, &status);
					for (matrix_col = 0; matrix_col < local_n; matrix_col++)
					{
						fprintf(fp, "%f\n", temp[matrix_col]);
					}
				}
			}
		}
		free(temp);
	}
	else
	{
		for (matrix_row = 0; matrix_row < local_m; matrix_row++)
		{
			MPI_Send(&loc_matrix_A[matrix_row*local_n], local_n, MPI_FLOAT, 0, 0, grid->comm);
		}
	}

}


void Fox_algorithm(
	GRID_INFO_T* grid,
	float* loc_matrix_A,
	float* loc_matrix_B,
	float* loc_matrix_C,
	int local_m,
	int local_n,
	int local_l)
{
	int stage;
	int source, dest;
	int k_bar;
	MPI_Status status;

	int i, j, k;
	for (i = 0; i < local_m; i++)
		for (j = 0; j < local_l; j++)
			loc_matrix_C[i*local_l + j] = 0.0;

	float *temp = malloc(local_m*local_n*sizeof(float));

	for (stage = 0; stage < grid->q; stage++)
	{
		k_bar = (grid->my_row + stage) % grid->q;
		if (k_bar == grid->my_col)
		{
			MPI_Bcast(loc_matrix_A, local_m*local_n, MPI_FLOAT, k_bar, grid->row_comm);
			for (i = 0; i < local_m; i++)
				for (j = 0; j < local_l; j++)
					for (k = 0; k < local_n; k++)
						loc_matrix_C[i*local_l + j] = loc_matrix_C[i*local_l + j] + loc_matrix_A[i*local_n + k] * loc_matrix_B[k*local_l + j];
		}
		else
		{
			MPI_Bcast(temp, local_m*local_n, MPI_FLOAT, k_bar, grid->row_comm);
			for (i = 0; i < local_m; i++)
				for (j = 0; j < local_l; j++)
					for (k = 0; k < local_n; k++)
						loc_matrix_C[i*local_l + j] = loc_matrix_C[i*local_l + j] + temp[i*local_n + k] * loc_matrix_B[k*local_l + j];
		}

		dest = (grid->my_row + grid->q - 1) % grid->q;
		source = (grid->my_row + 1) % grid->q;

		MPI_Sendrecv_replace(loc_matrix_B, local_n*local_l, MPI_FLOAT, dest, 0, source, 0, grid->col_comm, &status);
	}

	free(temp);
}


main(int argc, char* argv[])
{
	float* loc_matrix_A;
	float* loc_matrix_B;
	float* loc_matrix_C;
	int m, n, l;
	int local_m, local_n, local_l;
	double start_with_IO, end_with_IO, start_without_IO, end_without_IO;
	/* Start up MPI */
	MPI_Init(&argc, &argv);

	struct GRID_INFO_T *grid = malloc(sizeof(struct GRID_INFO_T));


	Setup_grid(grid);

	MPI_Barrier(MPI_COMM_WORLD);
	start_with_IO = MPI_Wtime();

	loc_matrix_A = read_matrix("data/A.txt", grid, &m, &n);
	loc_matrix_B = read_matrix("data/B.txt", grid, &n, &l);

	local_m = m / grid->q;
	local_n = n / grid->q;
	local_l = l / grid->q;

	loc_matrix_C = malloc(local_m*local_l*sizeof(float));

	MPI_Barrier(MPI_COMM_WORLD);
	start_without_IO = MPI_Wtime();

	Fox_algorithm(grid, loc_matrix_A, loc_matrix_B, loc_matrix_C, local_m, local_n, local_l);

	MPI_Barrier(MPI_COMM_WORLD);
	end_without_IO = MPI_Wtime();

	print_matrix("data/C.txt", grid, loc_matrix_C, m, l);

	MPI_Barrier(MPI_COMM_WORLD);
	end_with_IO = MPI_Wtime();

	if (grid->my_rank == 0)
	{
		printf("time with Input/Output = %lf\n", end_with_IO - start_with_IO);
		printf("time without Input/Output = %lf\n", end_without_IO - start_without_IO);
	}

	free(loc_matrix_A); // deallocating
	free(loc_matrix_B);
	free(loc_matrix_C);
	free(grid);

	MPI_Finalize();

	/* Shut down MPI */

}
