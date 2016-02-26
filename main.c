#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab3IO.h"
#include <omp.h>

#include "timer.h"

#define TOL 0.0005

int main(int argc, char * argv[]) {

//Need to record time

double start, end;

  int a_num_threads = atoi(argv[1]);

  int i, j, k, size;
  double ** Au;
  double * X;
  double error, Xnorm;
  int * index;

  /*Load the datasize and verify it*/
  Lab3LoadInput(&Au, &size);

  /*Calculate the solution by serial code*/
  X = CreateVec(size);
  index = (int*) malloc(size * sizeof(int));

  for(i = 0; i < size; ++i) {
    index[i] = i;
  }

  if(size == 1) {
    X[0] = Au[0][1] / Au[0][0];
  } else {

	GET_TIME(start)
#pragma omp parallel for num_threads(a_num_threads) shared(Au) private(i, j, k)
    /*Gaussian elimination*/
    for(k = 0; k < size - 1; ++k) {
      /*Pivoting*/
      double temp = 0;

      for(i = k, j = 0; i < size; ++i) {
	if (temp < Au[index[i]][k] * Au[index[i]][k]){
	  temp = Au[index[i]][k] * Au[index[i]][k];
	  j = i;
	}
      }

      if(j != k) {
	/* Swap */
	i = index[j];
	index[j] = index[k];
	index[k] = i;
      }

      /*calculating*/
      for(i = k + 1; i < size; ++i) {
	temp = Au[index[i]][k] / Au[index[k]][k];


	for(j = k; j < size + 1; ++j) {
	  Au[index[i]][j] -= Au[index[k]][j] * temp;
	}


      }       
    }


#pragma omp parallel for num_threads(a_num_threads) shared(Au, X) private(i, j, k)
    /*Jordan elimination*/
    for(k = size - 1; k > 0; --k) {
      for(i = k - 1; i >= 0; --i) {
	double temp = 0;
	temp = Au[index[i]][k] / Au[index[k]][k];
	Au[index[i]][k] -= temp * Au[index[k]][k];
	Au[index[i]][size] -= temp * Au[index[k]][size];
      } 
    }



    /*solution*/
    for(k=0; k< size; ++k) {
      X[k] = Au[index[k]][size] / Au[index[k]][k];
    }

  }

GET_TIME(end)

  //Gotta write out the matrix somewhere so we can compare.
  Lab3SaveOutput(X, size, end-start);


  DestroyVec(X);
  DestroyMat(Au, size);

  free((void*)index);

  return 0;
}
