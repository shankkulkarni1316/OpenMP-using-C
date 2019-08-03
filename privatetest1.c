#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100000

int main(int argc, char *argv) {
	omp_set_num_threads(1);//set number of threads here
	int i, j, x, tid;
	double start, end;			// used for timing

	start = omp_get_wtime(); 	//start time measurement
	#pragma omp parallel for private(x,tid)
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++) {
		tid = omp_get_thread_num();
		x=tid;
	} 
	end = omp_get_wtime();	//end time measurement
	printf("Time of parallel computation: %f seconds\n", end-start);

	return(0);
}

