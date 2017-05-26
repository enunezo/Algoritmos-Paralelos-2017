#include <stdio.h>

const int N = 10;

__global__
void matrixveecmultkernel(float *A, float *B,float *C, int n){
  int i = threadIdx.x+blockDim.x * blockIdx.x;
  if (i<n)
    C[i] += A[i*blockIdx.x+i]*B[i];
}

int main(){
  float hA[N][N], hB[N] , hC[N];
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
    hB[i] = rand() % 100;
    printf ("%4.2f \t", hB[i]);
  }
  printf ("\n");
  
  cudaMalloc((void**) &dA, N*N*sizeof(float));
  cudaMalloc((void**) &dB, N*sizeof(float));
  cudaMalloc((void**) &dC, N*sizeof(float));
  
  cudaMemcpy(dA,hA,N*N*sizeof(float),cudaMemcpyHostToDevice);
  cudaMemcpy(dB,hB,N*sizeof(float),cudaMemcpyHostToDevice);

  matrixveecmultkernel<<<   N/256, 256   >>>(dA, dB, dC, N);
  //
  cudaMemcpy(hC,dC,N*sizeof(float),cudaMemcpyDeviceToHost);
  cudaFree(dB);
  cudaFree(dC);
  cudaFree(dA);

  for (int i = 0; i < N; ++i){
    printf ("%4.2f \n", hC[i]);
  }
}