#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <gsl/gsl_rng.h>

#define MAXTHREADS 4
#define PADDING 16

int inside_unit_circle(double x, double y){
    return (x*x + y*y < 1);
}

int main(int argc, char **argv){
    if(argc != 3){
        printf("Wrong Input command! Must be 3!");
        exit(-1);
    }

    int nthd_req = atoi(argv[1]);
    int npicks = atoi(argv[2]);

    if(nthd_req > MAXTHREADS){
        printf("%d threads are max!", MAXTHREADS);
        exit(-1);
    }

    int i;

    omp_set_num_threads(nthd_req);

    double inside[MAXTHREADS][PADDING];
    double outside[MAXTHREADS][PADDING];

    for(i = 0; i < MAXTHREADS; i++){
        inside[i] = 0;
        outside[i] = 0;
    }

    double t1 = omp_get_wtime();
    #pragma omp parallel{
        int tid = omp_get_thread_num();
        int nthd = omp_get_num_threads();

        if(tid == 0){
            printf("Requested %d threads, got %d\n", nthd_req, nthd);
            if(npicks % nthd != 0){
                printf("Number of random picks not divisible by number of threads\n");
                exit(-1);
            }
        }

        const gsl_rng_type *T;
        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);

        long seed = abs(
            (
                (time(NULL)*181) * ((tid-83)*359)
            )%104729
        );
        gsl_rng_set(r, seed);
        for(int i=0; i<npicks/nthd; i++){
            double x = gsl_rng_uniform(r)*2.0-1.0;
            double y = gsl_rng_uniform(r)*2.0-1.0;

            if(inside_unit_circle(x, y)){
                inside[tid][0] += 1.0;
            }else{
                outside[tid][0] += 1.0;
            }
        }
    }

    printf("The parallel section took %f seconds to execute.\n", omp_get_wtime() - t1);
    double out = 0; double in = 0;
    for(int i=0; i<MAXTHREADS; i++){
        out += outside[i][0];
        in += inside[i][0];
    }
    printf("Our estimate of pi is %f\n", 4 * in / (in+out));
}