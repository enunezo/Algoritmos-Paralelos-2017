#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define MAXN 5

void Read_Vector(double  * local_a, int local_n, int n , int my_rank){
    MPI_Comm comm = MPI_COMM_WORLD;
    double * a = NULL ;
    int i;
    if(my_rank == 0){
        a = malloc(n*sizeof(double));
        printf("Ingresa los datos del vector:\n");
        for(i = 0 ; i < n ; ++i){
          scanf("%lf",&a[i]);
        }
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE,0,comm);
        free(a);
    } 
    else {
        MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
    }
}

void Print_Vector(double * local_b, int local_n, int n , int my_rank){
    MPI_Comm comm = MPI_COMM_WORLD;
    double * b = NULL ;
    int i;
    if(my_rank == 0){
        b = malloc(n * sizeof(double));
        MPI_Gather(local_b,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
        printf("******Impresion del vector**********\n");
        for(i = 0 ; i < n ; ++i){
          printf("%.1lf ", b[i]);
        }
        printf("\n");
        free(b);
    }
    else{
      MPI_Gather(local_b,local_n,MPI_DOUBLE, b, local_n, MPI_DOUBLE,0,comm);
    }
}

int main(){
    int my_rank,comm_sz,n;
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, & comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);
    n = 1000000;
    double *locala = malloc(sizeof(double) * (n/comm_sz+10)) ;
    
    Read_Vector(locala,n/comm_sz,n,my_rank);
    Print_Vector(locala,n/comm_sz,n,my_rank);

    MPI_Finalize();
    return 0;
}