#include <stdio.h>
#include <string.h>
#include "mpi.h"

void main(int argc, char* argv[]){
    int my_rank;
    int p;
    int source;
    int dest; 
    int tag=0;
    char message[100];
    MPI_Status status;

    // start the MPI world 
    MPI_Init(&argc, &argv);
    
    // Find out the process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Find out number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank){
        // create message
        // message takes the char data from 2nd argument
        sprintf(message, "Greetings from process %d!", my_rank);
        dest = 0; 
        MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }else{
        for(source=1; source<p; source++){
            MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
            printf("%s\n", message);
        }
    }

    MPI_Finalize();

}