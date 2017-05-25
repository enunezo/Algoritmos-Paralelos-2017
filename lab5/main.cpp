#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <iomanip>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

using namespace std;

#ifndef NUMM_TOSS
#define NUMM_TOSS 100
#endif

int threads_count = 10;

int main() {
  double pi_estimate,x,y;
  srand(time(NULL));

  long toss;
  long long int distance_squared, toss_in_circle;

  toss_in_circle = 0;
//  # pragma omp parallel for num_threads(threads_count) \
    reduction(+: toss_in_circle)
  for ( toss = 0; toss < 10 ; toss ++) {
    x= ((double(rand()) / double(RAND_MAX)) * 2) - 1;
    y= ((double(rand()) / double(RAND_MAX)) * 2) - 1;
    distance_squared = x*x + y*y ;
    if ( distance_squared <= 1) {
      toss_in_circle ++;
    }
  }
  pi_estimate= 4*toss_in_circle /((double) toss_in_circle );

  cout<<pi_estimate;
  return 0;
}
