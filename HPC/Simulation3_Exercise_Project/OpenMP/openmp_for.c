#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int i, my_rank;

int main(int argc, char **argv){
    int thread_count = strtol(argv[1], NULL, 10);

    #pragma omp parallel for num_threads(thread_count)
    for(i=0; i<5; i++){
        my_rank = omp_get_thread_num();
        printf("Hello from thread %d processing for loop index i=%d\n", my_rank, i); 
    }

    return 0;
}