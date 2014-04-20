/**
 *  cdf97.c - Fast discrete biorthogonal CDF 9/7 wavelet forward and inverse transform (lifting implementation)
 *  
 *  written by Di Wu - allwu@cs.ucla.edu
 *  based on the version from Gregoire Pau (2006) - gregoire.pau@ebi.ac.uk
 *
 *  removed global states for better parallelization
 *  only supports image sizes that are product of 2^k
 *
 *  2013.7
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cdf97.h"

#define a1 (-1.586134342)
#define a2 (-0.05298011854)
#define a3 (0.8829110762)
#define a4 (0.4435068522)

#define k1 1.1496043988602418
#define k2 (1/1.1496043988602418)

#define x(i, j)			x[(i)*M+(j)]
#define tempbank(i, j)	tempbank[(i)*M+(j)]

/**
 * predict and update
 *
 * computation is an even-odd update:
 * first update all odd columns based on even columns, then update even columns based on odd columns
 */
 
void init_environment()
{
	
}
 
void fwt97_pd(
		float* x, 
		float *tempbank,
		int n, int m,
		int N, int M)
{
	int i, j;

	for (i=0; i<n; i++) {
		// Predict 1
		for (j=1; j<m-2; j+=2) {
			x(i, j) += a1*(x(i, j-1) + x(i, j+1));
		} 
		x(i, m-1) += 2*a1*x(i, m-2);
		// Update 1
		for (j=2; j<m; j+=2) {
			x(i, j) += a2*(x(i, j-1) + x(i, j+1));
		}
		x(i, 0) += 2*a2*x(i, 1);
		// Predict 2
		for (j=1; j<m-2; j+=2) {
			x(i, j) += a3*(x(i, j-1) + x(i, j+1));
		}
		x(i, m-1) += 2*a3*x(i, m-2);

		// Update 2
		for (j=2; j<m; j+=2) {
			x(i, j) += a4*(x(i, j-1) + x(i, j+1));
		}
		x(i, 0) += 2*a4*x(i, 1);

		for (j=0; j<m; j+=16) {
				tempbank(i, j/2) = k1*x(i, j);
				tempbank(i, (j+1+m)/2) = k2*x(i, j+1);
				tempbank(i, (j+2)/2) = k1*x(i, j+2);
				tempbank(i, (j+3+m)/2) = k2*x(i, j+3);
				tempbank(i, (j+4)/2) = k1*x(i, j+4);
				tempbank(i, (j+5+m)/2) = k2*x(i, j+5);
				tempbank(i, (j+6)/2) = k1*x(i, j+6);
				tempbank(i, (j+7+m)/2) = k2*x(i, j+7);
				tempbank(i, (j+8)/2) = k1*x(i, j+8);
				tempbank(i, (j+9+m)/2) = k2*x(i, j+9);
				tempbank(i, (j+10)/2) = k1*x(i, j+10);
				tempbank(i, (j+11+m)/2) = k2*x(i, j+11);
				tempbank(i, (j+12)/2) = k1*x(i, j+12);
				tempbank(i, (j+13+m)/2) = k2*x(i, j+13);
				tempbank(i, (j+14)/2) = k1*x(i, j+14);
				tempbank(i, (j+15+m)/2) = k2*x(i, j+15);
		}
	
	}
}

/**
 * de-interleave with transpose
 *
 * two different function (foo and bar) are used to move data
 * between x and tempbank
 * the reason for this design is to avoid 'spilling' in x when data
 * is not square (n!=m)
 *
 **/
void fwt97_dl_foo (
		float* x, 
		float* tempbank, 
		int n, int m, 
		int N, int M) 
{
	int i, j, ii, jj;
    // de-interleave and transpose
	int tile_size = 8;
	int tile_count = M/tile_size;
	for (i=0; i<n; i+=tile_size) {
		for (j=0; j<m; j+=tile_size) {
			for (ii = 0; ii < tile_size; ii++){
				for (jj = 0; jj < tile_size; jj++){
					int new_i = i + ii;
					int new_j = j + jj;
					tempbank(new_j, new_i) = x(new_i, new_j);
				}
			}
		}
	}
}

/**
 *	CDF9/7 wavelet transform
 *
 *	this version assumes a column-based memory layout (Matlab default layout)
 *	so the wavelet along columns of the original matrix would be the transform
 *	along row in this code.
 *
 *	input matrix: nxm (n cols, m rows)
 *
 **/
void cdf97(
		float* x, 
		float* tempbank, 
		int n, int m, 
		int level
		) 
{

	int i;
	int nlevel = (level < 0 ? -level : level);

	int N = n;
	int M = m;

	int plevel = 1<<nlevel-1;

	// this means image is not big enough for transformation 
	// which we assume will not happen
	if (N/plevel < 2 || M/plevel < 2) 
		return;

	if (level > 0) {	
		// forward DWT (1. col; 2. row)
		for (i=0; i<nlevel; i++) {

			// Stage 1
			fwt97_pd(x, tempbank, m, n, M, N);
			// Stage 2
			fwt97_dl_foo(tempbank, x, m, n, M, N);
			// Stage 3
			fwt97_pd(x, tempbank, n, m, N, M);
			// Stage 4
			fwt97_dl_foo(tempbank, x,  n, m, N, M);
			
			// work on the upper left image (low pass filtered) in the next level
			n /= 2;
			m /= 2;
		}
	}
	else {				
		// backward DWT, omitted for the project
		;	
	}
}


