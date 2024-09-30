#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


//
// sharing.c -- example of consequences of sharing and false sharing
//

/*
 *  Compile with:
 *      gcc -Xpreprocessor -fopenmp -O3 -lomp sharing.c (clang MacOSX)
 *      gcc -fopenmp -O3 sharing.c (gcc)
 */

// Dimension of the array.  Data will be DIM x DIM
const int DIM = 4096*4096;
const int TRIALS = 4;


// Each thread writes to adjacent variables in same cache line
void sharing ( double* array, double* outar )
{
    #pragma omp parallel 
    {
       	for (int x=0; x<DIM; x++) {
            outar[0] += array[x];
	}
    }        
}

// Each thread writes to adjacent variables in same cache line
void false_sharing ( double* array, double* outar )
{
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
       	for (int x=0; x<DIM; x++) {
            outar[tid] += array[x];
	}
    }        
}

// Each thread writes to variables in different memory regions
void no_sharing ( double* array, double* outar )
{
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
       	for (int x=0; x<DIM; x++) {
            outar[tid*1024] += array[x];
	}
    }        
}


/* Helper function to deal with gettimeofday()

Subtract the `struct timeval' values X and Y,
    storing the result in RESULT.
    Return 1 if the difference is negative, otherwise 0. */
int timeval_subtract (struct timeval * result, struct timeval * y, struct timeval * x)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
    
  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

void initialize ( double* array )
{
    for (int x=0; x<DIM; x++) {
        array[x] = (double)rand()/RAND_MAX;
    }        
}


int main()
{
    double* rand_ar = (double*) malloc ( DIM * sizeof(double));
    double* out_ar = (double*) malloc ( DIM * sizeof(double));

    struct timeval begin, end, tresult;

    // warm up arrays w.r.t. caching
    initialize(rand_ar);

    // Compare iteration order w.r.t. memory layout
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        sharing(rand_ar, out_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("sharing = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );

        gettimeofday(&begin, NULL);
        false_sharing(rand_ar, out_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("false sharing = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );

        gettimeofday(&begin, NULL);
        no_sharing(rand_ar, out_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("no sharing = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }
}
