#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
double get_wall_time(){
  struct timeval time;
  if (gettimeofday(&time,NULL))
    return 0;
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void slidingAvg_host(int n, int* a, int* result) {
  int i;
  assert(n>0);
  for(i=0; i<n; i++)
    result += a[i];
}

void slidingAvg(int n, int* a, int* result) {
  int i;
  assert(n>0);
  #pragma acc parallel num_gangs(1) num_workers(1) copyin(a[n]) copy(result[n])
  {
    #pragma acc loop seq 
    // add sliding window average code here
    for(i=0; i<n; i++) {
      result += a[i];
    }
  }
}

int main() {
  int n, d_sum, h_sum, i;
  int* a, result, result2;
  double t0, t1, t2;
  n = 10*2<<20;
  a = (int*) malloc(n*sizeof(int));
  result = (int*) malloc(n*sizeof(int));
  result2 = (int*) malloc(n*sizeof(int));
  for(i=0; i<n; i++)
    a[i] = i;
  t0 = get_wall_time();
  slidingAvg(n,a,result);
  t1 = get_wall_time();
  slidingAvg_host(n,a,result2);
  t2 = get_wall_time();
  //compare the result arrays!
  //if( d_sum != h_sum ) {
  //  printf("ERROR sum device %d != sum host %d\n", d_sum, h_sum);
  //  return 1;
  //}
  printf("time device %f time host %f\n", t1-t0, t2-t1);
  free(a);
  free(result);
  free(result2);
  return 0;
}
