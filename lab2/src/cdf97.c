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
// System includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Lab includes
#include "cdf97.h"

// OpenCL includes
#include <CL/cl.h>

#define a1 (-1.586134342)
#define a2 (-0.05298011854)
#define a3 (0.8829110762)
#define a4 (0.4435068522)

#define k1 1.1496043988602418
#define k2 (1/1.1496043988602418)

#define x(i, j)			x[(i)*M+(j)]
#define tempbank(i, j)	tempbank[(i)*n+(j)]

const char* programSource = 
"__kernel\n"
"void vecadd(__global int *A,\n"
"__global int *B,\n"
"__global int *C)\n"
"{\n"
"\n"
"// Get the work-item's unique ID\n"
"int idx = get_global_id(0);\n"
"\n"
"// Add the corresponding locations of\n"
"// 'A' and 'B', and store the result in 'C'.\n"
"C[idx] = A[idx] + B[idx];\n"
"}\n"
;

/**
 * init GPU
 *
 * initializes context
 * Unable to get this program working. Compiler was
 * never able to find "CL/cl.h" header file and
 * therefore couldn't get even the simple setup
 * modeled after the example (below) to work properly.
 */
 
int init_context(
		float* A,
		float* B)
{
	    // This code executes on the OpenCL host
    
    // Host data
    A = NULL;  // Input array
    B = NULL;  // Output array
    
    // Elements in each array
    const int elements = 2048;   
    
    // Compute the size of the data 
    size_t datasize = sizeof(float)*elements;

    // Allocate space for input/output data
    A = (float*)malloc(datasize);
    B = (float*)malloc(datasize);

    // Initialize the input data
    // Insert stuff here

    // Use this to check the output of each API call
    cl_int status;  
     
    // Retrieve the number of platforms
    cl_uint numPlatforms = 0;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);

    // Allocate enough space for each platform
    cl_platform_id *platforms = NULL;
    platforms = (cl_platform_id*)malloc(
        numPlatforms*sizeof(cl_platform_id));
 
    // Fill in the platforms
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	cl_int platform_index = -1;
	char cBuffer[1024];
	int i;
	for (i=0; i<numPlatforms; i++) {
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
		if (strstr(cBuffer, "NVIDIA") != NULL) {
			platform_index = i;
			break;
		}
	}
	if (platform_index < 0) {
		printf("Cannot find platforms support OpenCL.\n");
		return -1;
	}
	else {
		printf("Selected platform '%s'. %d\n", cBuffer, platform_index);
	}

    // Retrieve the number of devices
    cl_uint numDevices = 0;
    status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_GPU, 0, 
        NULL, &numDevices);

    // Allocate enough space for each device
    cl_device_id *devices;
    devices = (cl_device_id*)malloc(
        numDevices*sizeof(cl_device_id));

    // Fill in the devices 
    status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_ALL,        
        numDevices, devices, NULL);

    // Create a context and associate it with the devices
    cl_context context;
    context = clCreateContext(NULL, numDevices, devices, NULL, 
        NULL, &status);

    // Create a command queue and associate it with the device 
    cl_command_queue cmdQueue;
    cmdQueue = clCreateCommandQueue(context, devices[0], 0, 
        &status);

    // Create a buffer object that will contain the data 
    // from the host array A
    cl_mem bufA;
    bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize,                       
       NULL, &status);

    // Create a buffer object that will hold the output data
    cl_mem bufB;
    bufB = clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize,
        NULL, &status); 
    
    // Write input array A to the device buffer bufferA
    status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 
        0, datasize, A, 0, NULL, NULL);

    // Create a program with source code
    cl_program program = clCreateProgramWithSource(context, 1, 
        (const char**)&programSource, NULL, &status);

    // Build (compile) the program for the device
    status = clBuildProgram(program, numDevices, devices, 
        NULL, NULL, NULL);

	// Create the vector addition kernel
    cl_kernel kernel;
    kernel = clCreateKernel(program, "vecadd", &status);

    // Associate the input and output buffers with the kernel 
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);

    // Define an index space (global work size) of work 
    // items for execution. A workgroup size (local work size) 
    // is not required, but can be used.
    size_t globalWorkSize[1];   
 
    // There are 'elements' work-items 
    globalWorkSize[0] = elements;

    // Execute the kernel for execution
    status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, 
        globalWorkSize, NULL, 0, NULL, NULL);

    // Read the device output buffer to the host output array
    clEnqueueReadBuffer(cmdQueue, bufB, CL_TRUE, 0, 
        datasize, B, 0, NULL, NULL);

    // Verify the output

    // Free OpenCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseContext(context);

    // Free host resources
    free(A);
    free(B);
    free(platforms);
    free(devices);

    return 0;
}

/**
 * predict and update
 *
 * computation is an even-odd update:
 * first update all odd columns based on even columns, then update even columns based on odd columns
 */
/*void fwt97_pd(
		float* x,
		float* tempbank,
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
	
		for (j = 0; j < m - 2; j += 8)
		{
			tempbank(i, j/2) = k1*x(i, j);
			tempbank(i, (j+2)/2) = k1*x(i,j+2);
			tempbank(i, (j+4)/2) = k1*x(i,j+4);
			tempbank(i, (j+6)/2) = k1*x(i,j+6);
		}
		
		for (j = 1; j < m - 2; j += 8)
		{
			tempbank(i, j/2+m/2) = k2*x(i, j);
			tempbank(i, (j+2)/2+m/2) = k2*x(i,j+2);
			tempbank(i, (j+4)/2+m/2) = k2*x(i,j+4);
			tempbank(i, (j+6)/2+m/2) = k2*x(i,j+6);
		}
	
	}
}*/
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

		for (j=0; j<m; j++) {
			if (j%2==0) 
				tempbank(i, j/2) = k1*x(i, j);
			else 
				tempbank(i, j/2+m/2) = k2*x(i, j);
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
			fwt97_dl_foo(tempbank, x, n, m, N, M);

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


