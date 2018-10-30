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

int reduce_sum_host(int n, int* a) {
  int result = 0;
  int i;
  assert(n>0);
  for(i=0; i<n; i++)
    result += a[i];
  return result;
}

int reduce_sum(int n, int* a) {
  int result = 0;
  int i;
  assert(n>0);
  #pragma acc parallel num_gangs(1) num_workers(1) copyin(a[n]) copy(result)
  {
    #pragma acc loop seq 
    for(i=0; i<n; i++) {
      result += a[i];
    }
  }
  return result;
}

int main() {
  int n, d_sum, h_sum, i;
  int* a;
  double t0, t1, t2;
  n = 10*2<<20;
  a = (int*) malloc(n*sizeof(int));
  for(i=0; i<n; i++)
    a[i] = i;
  t0 = get_wall_time();
  d_sum = reduce_sum(n,a);
  t1 = get_wall_time();
  h_sum = reduce_sum_host(n,a);
  t2 = get_wall_time();
  if( d_sum != h_sum ) {
    printf("ERROR sum device %d != sum host %d\n", d_sum, h_sum);
    return 1;
  }
  printf("time device %f time host %f\n", t1-t0, t2-t1);
  free(a);
  return 0;
}
