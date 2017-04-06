/* C Example */
#include <mpi.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
  int rank, size,num=0;

  MPI_Init (&argc, &argv);      /* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
  /* printf("%d es impreso por %d",num,rank); */
  do{
    if(num!=0 || rank==1 )
      MPI_Recv(&num, 1, MPI_INT, !rank , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf("%d es impreso por %d\n",num,rank);
    ++num;
    MPI_Send(&num, 1, MPI_INT, !rank, 0, MPI_COMM_WORLD);
  }while(num < 20);
  MPI_Finalize();
  return 0;
}