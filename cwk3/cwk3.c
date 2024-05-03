//
// Includes.
//
#include <stdio.h>
#include <stdlib.h>
#include "helper_cwk.h"			// Note this is not the same as the 'helper.h' used for examples.


//
// Main.
//
int main( int argc, char **argv )
{
	//
	// Initialisation.
	//
	
	// Initialise OpenCL. This is the same as the examples in lectures.
	cl_device_id device;
	cl_context context = simpleOpenContext_GPU(&device);

	cl_int status;
	cl_command_queue queue = clCreateCommandQueue( context, device, 0, &status );
	
	// Get the parameters (N = no. of nodes/gradients, M = no. of inputs). getCmdLineArgs() is in helper_cwk.h.
	int N, M;
	getCmdLineArgs( argc, argv, &N, &M );

	// Initialise host arrays. initialiseArrays() is defined in helper_cwk.h. 
	float
		*gradients = (float*) malloc( N* sizeof(float) ),
		*inputs    = (float*) malloc(   M* sizeof(float) ),
		*weights   = (float*) malloc( N*M* sizeof(float) );
	initialiseArrays( gradients, inputs, weights, N, M );
	

	// float *weights_test   = (float*) malloc( N*M*sizeof(float) );

	// for (int i=0; i<N *M ;i++){
	// 	weights_test[i] = weights[i];
	// }


	cl_mem
		d_gradients = clCreateBuffer( context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N* sizeof(float), gradients, &status ),
		d_inputs    = clCreateBuffer( context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,   M* sizeof(float), inputs, &status ),
		d_weights   = clCreateBuffer( context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, N*M* sizeof(float), weights, &status );
	if (!d_weights || !d_gradients || !d_inputs) {
        printf("Error: Failed to create buffers!\n");
        exit(1);
    }


	cl_kernel kernel = compileKernelFromFile("cwk3.cl", "cwk3", context, device);


	// set the kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_gradients);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_inputs);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_weights);
    clSetKernelArg(kernel, 3, sizeof(int), &M);
	

	// execute the kernel
	size_t global_work_size;
	clGetDeviceInfo( device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &global_work_size, NULL);


	size_t indexSpaceSize[1], workGroupSize[1];
	indexSpaceSize[0] = N*M;

	
	if (N*M < global_work_size){
		workGroupSize[0] = N*M;
	}
	else{
		workGroupSize[0] = global_work_size;
	}

	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, indexSpaceSize, workGroupSize, 0, NULL, NULL);

	// copy the data back to the host
	clEnqueueReadBuffer(queue, d_weights, CL_TRUE, 0, N*M*sizeof(float), weights, 0, NULL, NULL);

	// check if both arrays are the same, if yes, print message, if not print error message
	// if( memcmp( weights, weights_test, N*M*sizeof(float) ) == 0 )
	// 	printf("weights arrays are the same\n");
	// else
	// 	printf("weights arrays are different\n");
	

	// THIS PIECE OF .... TEST HAS COSTED ME HOURS!!!!	
	// for( int i=0; i<N; i++ )
	// {
	// 	for( int j=0; j<M; j++ )
	// 		{
	// 			weights_test[i*M+j] += gradients[i] * inputs[j];
	// 		}
	// }
	
	// displayWeights(weights_test, N, M);


	// Output result to screen. DO NOT REMOVE THIS LINE (or alter displayWeights() in helper_cwk.h); this will be replaced
	// with a different displayWeights() for the the assessment, so any changes you might make will be lost.
	displayWeights( weights, N, M) ;
									// DO NOT REMOVE.
	
	free( gradients );
	free( inputs    );
	free( weights   );

	clReleaseCommandQueue( queue   );
	clReleaseContext     ( context );


	// free( weights_test   );	


	return EXIT_SUCCESS;
}
