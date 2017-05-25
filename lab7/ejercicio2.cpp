#include <stdio.h>

const int N = 10;
const int blocksize = 7;

__global__
void matrixveecmultkernel(float *A, float *B,float *C, int n){
  int i = threadIdx.x+blockDim.x * blockIdx.x;
  if (i<n)
    C[i] += A[i*blockIdx+i]*B[i];
}

int main(){
  float hA[N][N], hB[N] , hC[N];
  float *dA, * dB , *dC;
  for(int i = 0 ; i < N ; ++i)
    for(int j = 0 ; j < N ; ++j)
      hA[i][j] = rand() % 100;
  for(int i = 0 ; i < N ; ++i)
      hB[i] = rand() % 100;
  
  cudaMalloc((void**) &dA, N*N*sizeof(float));
  cudaMalloc((void*) &dB, N*sizeof(float));
  cudaMalloc((void*) &dC, N*sizeof(float));
  
  cudaMemcpy(dB,hB,N*sizeof(float),cudaMemcpyHostToDevice);
  cudaMemcpy(dC,hC,N*sizeof(float),cudaMemcpyHostToDevice);

  matrixveecmultkernel<<<   N/256, 256   >>>(dA, dB, dC, N);
  //
  cudaMemcpy(hA,dA,N*sizeof(float),cudaMemcpyDeviceToHost);
  cudaFree(dB);
  cudaFree(dC);
  cudaFree(dA);
}