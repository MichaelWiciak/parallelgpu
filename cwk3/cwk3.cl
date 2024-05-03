__kernel void cwk3(__global float *gradients, __global float *inputs, __global float *weights, int M)
{
    int i = get_global_id(0);    
    printf("%i", i);
    weights[i] += gradients[i / M] * inputs[i % M];
}



