#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


//
// row_column.c -- subset of running example in stencil.s
//

/*
 *  stencil.c
 *
 *  This program has sucessively optimized implementations of stencil computations.
 *  It has functions to 
 *     * randomly intialize 2-d array with doubles in [0,1]
 *     * Average a stencil of 2*HWIDTH+1 for each cell
 *     * Sum two averaged stencils
 *
 *  This computing pattern is common in numerical simulations and similar
 *  to the computation of convulation functions in ML.
 *
 *  Compile with:
 *      gcc -Xpreprocessor -fopenmp -O3 -lomp stencil.c (clang MacOSX)
 *      gcc -fopenmp -O3 stencil.c (gcc)
 */

// Dimension of the array.  Data will be DIM x DIM
//const int DIM = 16384;
//const int DIM = 8192;
const int DIM = 4096;
// Number of trials.  Set to get desired confidence intervals.
const int TRIALS = 4;
// HWIDTH = 2 matches the unrolled code.  If you change, comparisons will break.
const int HWIDTH = 2;

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

/* Randomly Initialize in column major order. 
  Strided access should be less efficient. */
void initializeyx ( double* array )
{
    /* Initialize the array to random values */
    for (int y=0; y<DIM; y++) {
        for (int x=0; x<DIM; x++) {
            array[x*DIM+y] = (double)rand()/RAND_MAX;
        }        
    }
}

/* Randomly Initialize in row major order. 
  Sequential access should be more efficient. */
void initializexy ( double* array )
{
    /* Initialize the array to random values */
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            array[x*DIM+y] = (double)rand()/RAND_MAX;
        }        
    }
}


int main()
{
    double* rand_ar1 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* rand_ar2 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* avg_ar1 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* avg_ar2 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* sum_ar = (double*) malloc ( DIM * DIM * sizeof(double));

    struct timeval begin, end, tresult;

    // warm up arrays w.r.t. caching
    initializexy(rand_ar1);
    initializexy(rand_ar2);

    // Compare iteration order w.r.t. memory layout
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        initializexy(rand_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("xy = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );

        gettimeofday(&begin, NULL);
        initializeyx(rand_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("yx = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }
}
