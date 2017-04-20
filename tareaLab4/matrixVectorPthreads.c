#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define MAX 500

int A[MAX][MAX];
int x[MAX];
int y[MAX];
int thread_count;

void al_matrix()
{
	for(int i=0;i<MAX;i++){
		for(int j=0;j<MAX;j++){
			A[i][j] = i+j+1;
		}
	}
}

void al_vector()
{
	for(int i=0;i<MAX;i++){
		x[i] = i+1;
	}
}

void printmatrix(){
	for(int i=0;i<MAX;i++){
		for(int j=0;j<MAX;j++){
			cout << A[i][j] << " "; 
		}
		cout << endl;
	}	
}

void* Pth_mat_vect(void* rank) {
	long my_rank = (long) rank;
	int i, j;
	int local_m = MAX/thread_count;
	int my_first_row = my_rank * local_m;
	int my_last_row = (my_rank+1) * local_m - 1;

	for (i = my_first_row; i <= my_last_row; i++) {
		y[i] = 0.0;
		for (j = 0; j < MAX; j++)
			y[i] += A[i][j] * x[j];
	}

	return NULL;
}

int main()
{
	al_matrix();
	al_vector();

	long thread;
	pthread_t* thread_handles;

 	cin >> thread_count;       		

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for(thread=0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	printmatrix();
	
}