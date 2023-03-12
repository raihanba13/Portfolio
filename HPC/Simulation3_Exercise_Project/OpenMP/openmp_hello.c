#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);
void Hello2(void);

int main(int argc, char **argv){
    // input number of thread from cmd
    int thread_count = strtol(argv[1], NULL, 10);

    // this is wrong, you can not do this
    // #pragma omp parallel num_threads(thread_count){
    //     Hello();
    //     Hello2();
    // }
    
    #pragma omp parallel num_threads(thread_count)
        Hello();

    return 0;
}

void Hello(void){
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("Hello from thread %d of %d\n", my_rank, thread_count);
}

void Hello2(void){
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("### Pokemon! Gota catch them all! %d of %d\n", my_rank, thread_count);
}