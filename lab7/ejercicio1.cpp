#include <stdio.h>

const int N = 10;

__global__
void matrixaddkernel(float * A , float * B , float * C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n) 
    C[i] = A[i] + B[i];
}

__global__
void matrixvaddkernelthread(float* A , float* B , float* C, int n)
{
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n*n) 
    C[i] = A[i] + B[i];
}

__global__
void matrixaddkernelrow(float * A , float * B , float * C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n)
    for(int j =0 ; j < n ; ++j)
      C[i*n+j] = A[i*n+j] + B[i*n+j];

}

__global__
void matrixaddkernelcolumn(float * A , float * B , float * C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n) 
    for (int j = 0; j < n; ++j)
      C[j*n+i] = A[j*n+i] + B[j*n+i];
}

int main()
{
  float hA[N][N], hB[N][N] , hC[N][N];
  float *dA, * dB , *dC;
  for(int i = 0 ; i < N ; ++i){
    for(int j = 0 ; j < N ; ++j){
      hA[i][j] = rand() % 100;
      printf ("%4.2f \t", hA[i][j]);
    }
    printf ("\n");
  }
  printf ("\n");
  for(int i = 0 ; i < N ; ++i){
    for(int j = 0 ; j < N ; ++j){
      hB[i][j] = rand() % 100;
      printf ("%4.2f \t", hB[i][j]);
    }
    printf ("\n");
  }
  printf ("\n");
  cudaMalloc((void**) &dA, N*sizeof(float));
  cudaMalloc((void**) &dB, N*sizeof(float));
  cudaMalloc((void**) &dC, N*sizeof(float));
  
  cudaMemcpy(dA,hA,N*sizeof(float),cudaMemcpyHostToDevice);
  cudaMemcpy(dB,hB,N*sizeof(float),cudaMemcpyHostToDevice);

  //matrixaddkernel<<<   N/256, 256   >>>(dA, dB, dC,N);
  matrixvaddkernelthread<<<   N/256, 256   >>>(dA, dB, dC,N);
  //matrixaddkernelrow<<<   N/256, 256   >>>(dA, dB, dC,N);
  //matrixaddkernelcolumn<<<   N/256, 256   >>>(dA, dB, dC,N);
  //
  cudaMemcpy(hC,dC,N*sizeof(float),cudaMemcpyDeviceToHost);
  cudaFree(dB);
  cudaFree(dC);
  cudaFree(dA);

  for (int i = 0; i < N; ++i){
    for (int j = 0; j < N; ++j){
      /* code */
      printf ("%4.2f \t", hC[i][j]);
    }
    printf ("\n");
  }
  
}