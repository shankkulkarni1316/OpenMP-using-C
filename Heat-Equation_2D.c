#include <omp.h>
#include <stdio.h>
#include <stdlib.h>





int main (int argc, char *argv[]) {

	
	int current, next, MAX_ITERATIONS, current1, next1;
	int S1, S2, O1;
	int i,j,iteration;
	int N, T;
	float error;   // to calculate error
	double start, end, time, start1, end1, time1, S; // used for timing
	int nthreads;

	nthreads = 2; // set number of threads

	omp_set_num_threads (nthreads); //setting number of threads

	// asking for input
	printf("Enter number of points in each dimension:");
	scanf("%d",&N);
	printf("Enter number of iterations:");
	scanf("%d",&T);

	double h[2][N][N];
	double hp[2][N][N];
	MAX_ITERATIONS = T;

	current = 0;
	next = 1;
	current1 = 0;
	next1 = 1;

	// Define boundary conditions

	for (i = 0; i < N; i++) {  // define all walls to 20 first
		h[current][0][i] = 20;
		h[current][N-1][i] = 20;
		h[current][i][0] = 20;
		h[current][i][N-1] = 20;

		h[next][0][i] = 20;
		h[next][N-1][i] = 20;
		h[next][i][0] = 20;
		h[next][i][N-1] = 20;
	}

	S1 = N*0.4;
	S2 = N*0.6;
	for (i = S1; i < S2+1; i++) {                // adding temperature for fireplace
		h[current][0][i] = h[current][0][i] + 80;
		h[next][0][i] = h[next][0][i] + 80;
	}

	// initial guess
	for (i = 1; i < N-1; i++) {
		for (j = 1; j < N-1; j++) {
			h[current][i][j] = 20;
		}
	}


// initial distribution for parallel code

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			hp[current1][i][j] = h[current][i][j];
			hp[next1][i][j] = h[next][i][j];
		}
	}

	  
//-------------------------------------------------------
	// printing the initial distribution
	O1 = N*0.1;
	printf("Initial temperature distribution:");
	printf ("\n");
		for (i = 0; i < N; ) {
			for (j = 0; j < N; ) {
				printf ("\t%.2f", hp[current1][i][j]);
				j = j + O1;

			}
			i = i +O1;
			printf ("\n");
		}

//--------------------------------------------------------
// Sequential part-----------------------------------------
	start = omp_get_wtime(); //start time measurement
	for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
		for (i = 1; i < N-1; i++) {
			for (j = 1; j < N-1; j++) {
				h[next][i][j] = 0.25 * (h[current][i-1][j] + h[current][i+1][j]
			                     + h[current][i][j-1] + h[current][i][j+1]);

				if (i==(1) && j==(N*0.5)) {
				error = h[next][i][j] - h[current][i][j];
				}
			}
		}
		current = next; // swap values of current and next
		next = 1 - current;
	}
	end = omp_get_wtime(); //end time measurement
	time=end-start;
//-----------------------------------------------------------

//-------------------------------------------------------------
// Parallel part---------------------------------------------

	
	

	start1 = omp_get_wtime(); //start time measurement
	for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
		#pragma omp parallel private(j,i)
		{
		#pragma omp for schedule (static)
		for (i = 1; i < N-1; i++) {
			
			for (j = 1; j < N-1; j++) {
				hp[next1][i][j] = 0.25 * (hp[current1][i-1][j] + hp[current1][i+1][j]
			                     + hp[current1][i][j-1] + hp[current1][i][j+1]);

				
			}
		}
		}
		current1 = next1; // swap values of current and next
		next1 = 1 - current1;
	}
	end1 = omp_get_wtime(); //end time measurement
	
	time1=end1-start1;
//----------------------------------------------------------------
//-------------------------------------------------------------

	// printing the output
	printf("Final temperature distribution:");
	printf ("\n");

	for (i = 0; i < N; ) {
		for (j = 0; j < N; ) {
			printf ("\t%.2f", h[current][i][j]);
			j = j + O1;

		}
		i = i +O1;
		printf ("\n");
	}


	printf("Error equal to =%f", error);
	printf("\n");
	
	printf("Time of sequential computation: %f seconds\n", time);
	printf("Time of parallel computation: %f seconds\n", time1);
	S = time/time1;
	printf("Speed-up factor is : %f\n", S);
	printf("Number of threads used: %d\n", nthreads);


	// check sequential and parallel versions give same answers, within rounding
	int err = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)   {
			if ((h[current][i][j] - hp[current1][i][j] > 0.001) || (hp[current1][i][j] - h[current][i][j] > 0.001))
				err = -1;
				}
	}
	if (err == -1) printf("ERROR, sequential and parallel versions give different answers\n");
	return(0);
}
