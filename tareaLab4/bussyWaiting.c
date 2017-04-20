#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int thread_count;

int counter;

void* func(void* rank) {
	counter++;
	printf("Counter value: %d\n",counter);
	while (counter < thread count);
}

int main(){
	long thread;
	pthread_t* thread_handles;

 	cin >> thread_count;       		

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for(thread=0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, func, (void*) thread);

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);
}