#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double sum;

/*pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]*/

void* threadSum(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }

   return NULL;
} 

double serialPi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}


void usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}

void getArgcs(int argc, char* argv[]) {
   if (argc != 3) usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS) usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) usage(argv[0]);
}  

int main(int argc, char* argv[]) {
   long       thread;  
   pthread_t* thread_handles;

   /* Get number of threads from command line */
   getArgcs(argc, argv);

   thread_handles = malloc (thread_count*sizeof(pthread_t));

   sum = 0.0;
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          threadSum, (void*)thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   sum = 4.0*sum;

   printf("With n = %lld terms,\n", n);
   printf("   Our estimate of pi = %.15f\n", sum);
   sum = serialPi(n);
   printf("   Single thread est  = %.15f\n", sum);
   printf("                   pi = %.15f\n", 4.0*atan(1.0));

   free(thread_handles);
   return 0;
} 
