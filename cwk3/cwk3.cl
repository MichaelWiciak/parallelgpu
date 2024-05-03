// Implement the kernel (or kernels), note this will be in OpenCL C
// the kernel will try to execute
// 	for( int i=0; i<N; i++ )
//	{
//		for( int j=0; j<M; j++ )
//			{
//				weights_test[i*M+j] += gradients_test[i] * inputs_test[j];
//			}
//	}
// but in parallel
// Implement the kernel (or kernels), note this will be in OpenCL C
// the kernel will try to execute
// 	for( int i=0; i<N; i++ )
//	{
//		for( int j=0; j<M; j++ )
//			{
//				weights_test[i*M+j] += gradients_test[i] * inputs_test[j];
//			}
//	}
// but in parallel
__kernel void cwk3(__global float* weights, __global float* gradients, __global float* inputs, int M, int N)
{
    int i = get_global_id(0);
    
    

    weights[i * M + j] += gradients[i] * inputs[j];
    
}



