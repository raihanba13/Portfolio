#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <gsl/gsl_rng.h>


#define MAXTHREADS 4
#define PADDING 16

int inside_unit_circle(double x , double y )
{
  return (x*x + y*y<1);
}
int main(int argc , char * argv[]) {
  int nth_req = atoi(argv[1]);
  int npicks = atoi(argv[2]);

  if (nth_req > MAXTHREADS)
  {
    printf("you can request at most %d threads\n" , MAXTHREADS);
    exit(-1);
  }

  omp_set_num_threads(nth_req);
  double inside[MAXTHREADS][PADDING];
  double outside[MAXTHREADS][PADDING];
  for(int i=0 ; i<MAXTHREADS ; i++)
    {
      inside[i]=0;
      outside[i]=0;
    }
  double t1 = omp_get_wtime();
  #pragma omp parallel 
  {
    int tid = omp_get_thread_num();
    int nth = omp_get_num_threads();

    if (tid==0)
    {
      printf("Requested thread is %d and got thread is %d\n",nth_req , nth);
      if(npicks% nth != 0)
      {
        printf("number of npicks should be divisible by number of threads \n");
        exit(-1);
      }
    }
    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r= gsl_rng_alloc(T);
    long seed = abs(((time(NULL) * 181) * ((tid-83) * 359)) % 104729);
    gsl_rng_set(r,seed);

    for(int i = 0 ; i< npicks/nth ; i++)
      {
        double x= gsl_rng_uniform(r) * 2.0 - 1.0;
        double y = gsl_rng_uniform(r) * 2.0 - 1.0;

        if(inside_unit_circle(x,y))
        {
          inside[tid][0]+=1.0;
        }else {
          outside[tid][0]+=1.0;
            }
      }
  }
  printf("the parallel section took %f seconds \n" , omp_get_wtime()-t1);
  double in =0;
  double out=0;

  for(int i =0 ; i<MAXTHREADS ; i++)
    {
      in+= inside[i][0];
      out+= outside[i][0];

      printf("the value of pi is %f\n", 4 * in / (in + out));
    }
}