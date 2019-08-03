#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "X11Macros.h" // X11 macros
#define X_RESN 800 // x resolution
#define Y_RESN 800 // y resolution


//double h[2][100][100];


int main (void) {

	int x,y;
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


/* --------------------------- X11 graphics setup ------------------------------ */
	initX11(X_RESN,Y_RESN); // includes the X11 initialization code
	 for (x = 0; x < N; x = x + 1) {
		for (y = 0; y < N; y = y + 1) {
			
			
			if (h[current][x][y] > 90) {
			 	XSetForeground(display,gc,WHITE); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 80) && (h[current][x][y] <= 90)) {
			 	XSetForeground(display,gc,GREEN); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 70) && (h[current][x][y] <= 80)) {
			 	XSetForeground(display,gc,MAGENTA); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 60) && (h[current][x][y] <= 70)) {
			 	XSetForeground(display,gc,VIOLET); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 50) && (h[current][x][y] <= 60)) {
			 	XSetForeground(display,gc,RED); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 40) && (h[current][x][y] <= 50)) {
			 	XSetForeground(display,gc,PINK); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] > 30) && (h[current][x][y] <= 40)) {
			 	XSetForeground(display,gc,ORANGE); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
			if ((h[current][x][y] >= 20) && (h[current][x][y] <= 30)) {
			 	XSetForeground(display,gc,BLUE); // color of foreground (applies to object to be drawn)

				XDrawPoint (display, win, gc, y, x); // draw point at location x, y in window
			}
	
			 

			 XFlush(display); // necessary to write to display

		
			 usleep(10); // provide a delay beween each drawing
		}
	 } 




	return(0);
}
