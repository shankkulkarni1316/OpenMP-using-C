#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


//double h[2][100][100];


int main (void) {
	int current, next, MAX_ITERATIONS;
	int S1, S2, O1;
	int i,j,iteration;
	int N, T;
	float error;

	// asking for input
	printf("Enter number of points in each dimension:");
	scanf("%d",&N);
	printf("Enter number of iterations:");
	scanf("%d",&T);

	double h[2][N][N];
	MAX_ITERATIONS = T;

	current = 0;
	next = 1;

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
//-------------------------------------------------------
	// printing the initial distribution
	O1 = N*0.1;
	printf("Initial temperature distribution:");
	printf ("\n");
		for (i = 0; i < N; ) {
			for (j = 0; j < N; ) {
				printf ("\t%.2f", h[current][i][j]);
				j = j + O1;

			}
			i = i +O1;
			printf ("\n");
		}

//--------------------------------------------------------

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
	return(0);
}
