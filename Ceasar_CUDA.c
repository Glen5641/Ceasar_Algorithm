#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>

/*
 * ceasar.c -- ceasar algorithm program with CUDA
 *
 * <Timed>
 * Scatter an array of characters across the processes and shift
 * them by k characters. Return the character array to master
 * <Timed>
 * and shift them back while checking integrity of array.
 *
 * Input: none.
 * Output: contents of rank 0 messages
 *
 * Modified by Clayton Glenn.
 */

#define N 33

  __global__ encrypt(char *p, char *c, int a, int b) {
    int tid = threadIdx.x;
    c[tid] = (a*p[tid] + b) % 256;
  }

int main (void) {

  //allocate memory on host
  charh_p[N] = “Plain text : This is an example ”;
  charh_c[N]  = “                                “;
  char*dev_p, *dev_c;
  inth_a = 171, h_b = 55;
  inth_k = 3, h_d = 91;

  //allocate the memory on the GPU
  cudaMalloc( (void**)&dev_p, N*sizeof(char));
  cudaMalloc( (void**)&dev_c, N*sizeof(char));

  //copy the arrays h_p and h_c to the GPU
  cudaMemcpy(dev_p, h_p, N*sizeof(char), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_c, h_c, N*sizeof(char), cudaMemcpyHostToDevice);
  encrypt <<<1, N – 1 >>>(dev_p, dev_c, a, b);

  //copy the result in array dev_c back to the host
  cudaMemcpy(h_c, dev _c, N*sizeof(char), cudaMemcpyDeviceToHost);

  //display results
  Printf(“ The given plain text is: \n %s\n”, *h_p);
  printf(“ The encrypted text is : \n %s\n”, *h_c);

  // Release the memory allocated on GPU
  cudaFree(dev_p);
  cudaFree(dev_c);

  return (0);
}
