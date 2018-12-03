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
    //result[i]=a[i];
  }
}

void slidingAvg(int n, int* a, int* result) {
  assert(n>0);
  #pragma acc parallel num_gangs(1) num_workers(1) copyin(a[n]) copy(result[n])
    #define rows n/2
    #define cols 2
    #pragma acc parallel loop num_gangs(1) num_workers(1) vector_length(1) 
    #pragma openarc transform window (a,result)
    for (int index=0; index<rows*cols;++index){
      int n=a[index-cols];
      int s=a[index+cols];
      int e=a[index+1];
      int w=a[index-1];
      result[index]=(a[index]+n+s+e+w)/5;
    }
  result[0]=result[1]=result[n-1]=result[n-2]=0; 
  
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
