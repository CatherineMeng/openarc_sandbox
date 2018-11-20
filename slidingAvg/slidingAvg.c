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
  result[0]=result[1]=result[n-2]=result[n-1]=0;
  assert(n>0);
  for(i=2; i<n-2; i++){
    result[i] = (a[i-2]+a[i-1]+a[i]+a[i+1]+a[i+2]);
    result[i]/=5;
  }
}

void slidingAvg(int n, int* a, int* result) {
  assert(n>0);
  #pragma acc parallel num_gangs(1) num_workers(1) copyin(a[n]) copy(result[n])
  {
    int i;
    int shreg[5] = {0,0,1,2,3};
    result[0]=result[1]=result[n-1]=result[n-2]=0;
    #pragma acc loop seq 
    for(i=2; i<n-2; i++) {
      for (int ind=0; ind<4; ind++){
        shreg[ind]=shreg[ind+1];
      }
      shreg[4]=a[i+2];
      result[i]=(shreg[0]+shreg[1]+shreg[2]+shreg[3]+shreg[4])/5;
    }
  }
}

int main() {
  int n, i;
  int* a; 
  int* result; 
  int* result2;
  double t0, t1, t2;
  n=10*2<<20;
  a = (int*) malloc(n*sizeof(int));
  result = (int*) malloc(n*sizeof(int));
  result2 = (int*) malloc(n*sizeof(int));
  for(i=0; i<n; i++)
  {  
    a[i] = i;
    result[i]=0;
    result2[i]=0;

  }
  t0 = get_wall_time();
  slidingAvg(n,a,result);
  t1 = get_wall_time();
  slidingAvg_host(n,a,result2);
  t2 = get_wall_time();
  for (i=0;i<n;i++){
    if (result[i]!=result2[i]){
      printf("Error device result!=host result at index %d\n", i);
      printf("Device result: %d\n",result[i]);
      printf("Host result: %d\n", result2[i]);
      return 1;
    }
  }
  printf("time device %f time host %f\n", t1-t0, t2-t1);
  free(a);
  free(result);
  free(result2);
  return 0;
}
