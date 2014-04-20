// This program implements a vector addition using OpenCL

// System includes
#include <stdio.h>
#include <stdlib.h>

// OpenCL includes
#include <CL/cl.h>

// OpenCL kernel to perform an element-wise addition 
const char* programSource =
"__kernel                                            \n"
"void vecadd(__global int *A,                        \n"
"            __global int *B,                        \n"
"            __global int *C)                        \n"
"{                                                   \n"
"                                                    \n"
"   // Get the work-item’s unique ID                 \n"
"   int idx = get_global_id(0);                      \n"
"                                                    \n"
"   // Add the corresponding locations of            \n"
"   // 'A' and 'B', and store the result in 'C'.     \n"
"   C[idx] = A[idx] + B[idx];                        \n"
"}                                                   \n"
;

int main() {
    
    // Use this to check the output of each API call
    cl_int status;  
     
    // Retrieve the number of platforms
    cl_uint numPlatforms = 0;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);

	printf("Found %d platforms support OpenCL.\n", numPlatforms, status);
 
    // Allocate enough space for each platform
    cl_platform_id *platforms = NULL;
    platforms = (cl_platform_id*)malloc(
        numPlatforms*sizeof(cl_platform_id));
 
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	for (int i = 0; i < numPlatforms; i++)
	{
		char cBuffer[1024];

		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
		printf("Platform %d: %s\n", i, cBuffer);

		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(cBuffer), cBuffer, NULL);
		printf("\tVendor: %s\n", cBuffer);

		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(cBuffer), cBuffer, NULL);
		printf("\tVersion: %s\n", cBuffer);
	
		// Retrieve the number of devices
		cl_uint numDevices = 0;
		status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, 
				NULL, &numDevices);
		printf("\tFound %d devices\n", numDevices);

		// Allocate enough space for each device
		cl_device_id *devices;
		devices = (cl_device_id*)malloc(
				numDevices*sizeof(cl_device_id));

		// Fill in the devices 
		status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL,        
				numDevices, devices, NULL);

		for(unsigned int d = 0; d < numDevices; ++d ) {
			clGetDeviceInfo(devices[d], CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
			printf("\tDevice %d:\t%s\n", d, cBuffer);

			cl_device_type type;
			clGetDeviceInfo(devices[d], CL_DEVICE_TYPE, sizeof(type), &type, NULL);
			printf("\tDevice Type:\t");
			if( type & CL_DEVICE_TYPE_CPU )
				printf("%s\n", "CL_DEVICE_TYPE_CPU");
			else if( type & CL_DEVICE_TYPE_GPU )
				printf("%s\n", "CL_DEVICE_TYPE_GPU");
			else if( type & CL_DEVICE_TYPE_ACCELERATOR )
				printf("%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
			else if( type & CL_DEVICE_TYPE_DEFAULT )
				printf("%s\n", "CL_DEVICE_TYPE_DEFAULT");
    
			// CL_DEVICE_MAX_COMPUTE_UNITS
			cl_uint compute_units;
			clGetDeviceInfo(devices[d], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);
			printf("\t\tCL_DEVICE_MAX_COMPUTE_UNITS:\t%u\n", compute_units);

			// CL_DEVICE_MAX_CLOCK_FREQUENCY
			cl_uint clock_frequency;
			clGetDeviceInfo(devices[d], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);
			printf("\t\tCL_DEVICE_MAX_CLOCK_FREQUENCY:\t%u MHz\n", clock_frequency);
		}
		free(devices);
	}

    // Free OpenCL resources

    // Free host resources
	free(platforms);

    return 0;
}

