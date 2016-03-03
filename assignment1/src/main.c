
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


int main (int argc, char *argv[]) {
  if (argc != 8) {
    fprintf(stderr, "Usage: %s <Number-Producers> <Number-Consumers> <Number-Products> <Number-sizeOfQueue>"
	    "<Number-SchedulingAlgo> <Number-QuantumnValue> <Number-RandomSeed>\n", argv[0]);
    exit (1);

  }

  /*  
  int noOfProducers = atoi(argv[1]);
  int noOfConsumers = atoi(argv[2]);
  int noOfProducts1 = atoi(argv[3]);
  noOfProducts = noOfProducts1;
  noOfProductToConsume = noOfProducts1;
  // noOfProductToConsume = noOfProducts = atoi(argv[3]);
  int sizeOfQueue1 = atoi(argv[4]);
  sizeOfQueue = sizeOfQueue1;
  int schedulingAlgo1 = atoi(argv[5]);
  schedulingAlgo = schedulingAlgo1;
  int quantumnValue1 = atoi(argv[6]);
  quantumnValue = quantumnValue1;
  int randomSeed = atoi(argv[7]);
  int i =0;
  
  pthread_t producers[noOfProducers];
  pthread_t consumers[noOfConsumers];
 
  srand(randomSeed);
  */

  return 0;
}
