#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 256

int main(int argc, char *argv) {
	omp_set_num_threads(4);//set number of threads here
	int i, j, k, x, z;
	int R;                 // number of runs
	double sum;
	double start, end, start1, end1;			// used for timing
	double A[N][N], B[N][N], C[N][N], D[N][N];	
	double time1[10], time2[10];
	double sumtime1, sumtime2, avgtime1, avgtime2;


	sumtime1 = 0;
	sumtime2 = 0;
	R=10;     // number of runs = 10

	// set some initial values for A and B
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)     {
			A[i][j] = j*1;
			B[i][j] = i*j+2;
		}
	}

	for (z = 0; z < R; z++) {
		// sequential matrix multiplication
		start = omp_get_wtime(); 	//start time measurement
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)   {
				sum = 0;
				for (k=0; k < N; k++) {
					sum += A[i][k]*B[k][j]; 
				}      
				C[i][j] = sum;
			}
		}
	 	end = omp_get_wtime();	//end time measurement
		time1[z]=end-start;
		//printf("Time of sequential computation: %f seconds\n", end-start);

		// parallel matrix multiplication here result in array D[N][N]
		start1 = omp_get_wtime(); 	//start time measurement
			
		
			
		for (i = 0; i < N; i++) {
			#pragma omp parallel private(sum,j,k)
			{
				#pragma omp for schedule (static)
				for (j = 0; j < N; j++)   {
					sum = 0;
					for (k=0; k < N; k++) {
						sum += A[i][k]*B[k][j]; 
					}      
					D[i][j] = sum;
				}
			}
		}

		
		end1 = omp_get_wtime();	//end time measurement
		time2[z]=end1-start1;
		//printf("Time of parallel computation: %f seconds\n", end1-start1); 	
		
	

	
		// check sequential and parallel versions give same answers, within rounding
		int error = 0;
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)   {
				if ((C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001))
					error = -1;
	
			}
		}
		if (error == -1) printf("ERROR, sequential and parallel versions give different answers\n");
	}
	
	for (i = 0; i < R; i++) {
		sumtime1 = sumtime1 + time1[i]; 
		sumtime2 = sumtime2 + time2[i];
	}

	avgtime1 = sumtime1/R;
	avgtime2 = sumtime2/R;

	printf("Time of sequential computation: %f seconds\n", avgtime1);
	printf("Time of parallel computation: %f seconds\n", avgtime2);
	return(0);
}

