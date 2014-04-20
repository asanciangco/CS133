// Minimize data transfer by having a small buffer containing the other args
// such as:
//		n, m, N, and M
//
// arg: x
//		must be read and write, since data is read from it as well as written
// arg: tempbank
//		must be write only, it's simply an output

#define a1 (-1.586134342)
#define a2 (-0.05298011854)
#define a3 (0.8829110762)
#define a4 (0.4435068522)

#define k1 1.1496043988602418
#define k2 (1/1.1496043988602418)

#define x(i, j)			x[(i)*M+(j)]
#define tempbank(i, j)	tempbank[(i)*M+(j)]

__kernel __attribute__ ((reqd_work_group_size(256, 1, 1)))
void fwd97_pd(__global float* x,
			  __global float* tempbank,
			  __global int*	  args)
{

////////////////////////////////////////////////
// Set up variables
////////////////////////////////////////////////

// strip the parameters out of the 'args' buffer
	int n = args[0];
	int m = args[1];
	int N = args[2];
	int M = args[3];
	
// set 'i' index based on location in work group
	int i = get_global_id(0);
	int j;
	
////////////////////////////////////////////////
// Function body
////////////////////////////////////////////////
	
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