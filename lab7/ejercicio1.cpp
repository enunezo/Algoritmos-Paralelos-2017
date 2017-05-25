#include <stdio.h>

const int N = 10;
const int blocksize = 7;

__global__
void matrixaddkernel(float * A , float * B , float * C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n) 
    C[i] = A[i] + B[i];
}

int main()
{
  float hA[N][N], hB[N][N] , hC[N][N];
  float *dA, * dB , *dC;
  for(int i = 0 ; i < N ; ++i)
    for(int j = 0 ; j < N ; ++j)
      hA[i][j] = rand() % 100;
  for(int i = 0 ; i < N ; ++i)
    for(int j = 0 ; j < N ; ++j)
      hB[i][j] = rand() % 100;
  
  cudaMalloc((void**) &dA, N*N*sizeof(float));
  cudaMalloc((void**) &dB, N*N*sizeof(float));
  cudaMalloc((void**) &dC, N*N*sizeof(float));
  
  cudaMemcpy(dA,hA,N*sizeof(float),cudaMemcpyHostToDevice);
  cudaMemcpy(dB,hB,N*sizeof(float),cudaMemcpyHostToDevice);

  matrixaddkernel<<<   N/256, 256   >>>(dA, dB, dC,N);
  //
  cudaMemcpy(hC,dC,N*sizeof(float),cudaMemcpyDeviceToHost);
  cudaFree(dB);
  cudaFree(dC);
  cudaFree(dA);
  
}