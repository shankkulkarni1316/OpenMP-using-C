#include <omp.h>
#include <stdio.h>
#include <stdlib.h>





int main (int argc, char *argv[]) {

	
	int current, next, MAX_ITERATIONS, current1, next1;
	int S1, S2, O1;
	int i,j,iteration;
	int N, T;
	float e1;  // to calculate max error
	int T1; // to count number of iterations
	float acc; // input from user for termination condition error

	double start, end, time, start1, end1, time1, S; // used for timing
	int nthreads;

	nthreads = 2; // set number of threads

	omp_set_num_threads (nthreads); //setting number of threads

	// asking for input
	printf("Enter number of points in each dimension:");
	scanf("%d",&N);
	printf("Enter number of iterations:");
	scanf("%d",&T);
	printf("Enter expected accuracy:");  // asking user to enter accuracy 
	scanf("%f",&acc);

	double h[2][N][N];
	double hp[2][N][N];
	double err[N][N];

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
			//err[i][j] = 0;
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
		#pragma omp parallel private(j)
		{
		#pragma omp for schedule (static)
		for (i = 1; i < N-1; i++) {
			
			for (j = 1; j < N-1; j++) {
				hp[next1][i][j] = 0.25 * (hp[current1][i-1][j] + hp[current1][i+1][j]
			                     + hp[current1][i][j-1] + hp[current1][i][j+1]);
				err[i][j] = (hp[next1][i][j] - hp[current1][i][j]);  // Calculating the error for each point
			
			}
		}
		}
		current1 = next1; // swap values of current and next
		next1 = 1 - current1;
		
		for (i = 1; i < N-1; i++) {
			for (j = 1; j < N-1; j++) {
				if (err[i][j] >= e1) {
					e1 = err[i][j];
				}
			}
		}



	T1 = iteration;  // count of iterations
	if (e1 <= acc)  // if error is less than accuracy, break the iteration loop
	break;
	else
	e1 = 0;	  // otherwise again initiate e1 value to 0 for next iteration
	


	}

	end1 = omp_get_wtime(); //end time measurement
	
	time1=end1-start1;
//----------------------------------------------------------------
//-------------------------------------------------------------

	// printing the output
//	printf("Final temperature distribution from sequential:");
//	printf ("\n");

//	for (i = 0; i < N; ) {
//		for (j = 0; j < N; ) {
//			printf ("\t%.2f", h[current][i][j]);
//			j = j + O1;

//		}
//		i = i +O1;
//		printf ("\n");
//	}

	printf("Final temperature distribution from parallel:");
	printf ("\n");

	for (i = 0; i < N; ) {
		for (j = 0; j < N; ) {
			printf ("\t%.2f", hp[current1][i][j]);
			
			j = j + O1;

		}
		i = i +O1;
		printf ("\n");
	}




	
	printf("Time of sequential computation: %f seconds\n", time);
	printf("Time of parallel computation: %f seconds\n", time1);
	S = time/time1;
	printf("Speed-up factor is : %f\n", S);
	printf("Number of threads used: %d\n", nthreads);

	printf("Number of iterations executed %d\n", T1);
	printf("Max error: %f\n", e1);

	

	return(0);
}

