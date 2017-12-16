/*Header file*/
#ifndef _ASSIGNMENT1_H_
#define _ASSIGNMENT1_H_

#include<stdio.h>
#include<pthread.h>
#include<queue>
#include<list>
#include<pthread.h>
#include<ctime>
#include<cstdlib> 
#include<stdlib.h>
#include<queue>
#include<sys/types.h>
#include<sys/time.h>
#include<algorithm>
#include<cfloat>
#include<unistd.h>
#include<iostream>
#include<string>
#include<vector>

using namespace std;
struct Product
{
  int id;
  int life;
  float timestamp;
};
int nprod;
int maxprod;
int quantum;
int usedprod;
float mincont;
float maxwaitt;
float maxcont;
float totalt;
float minwaitt;
unsigned int qsize;
unsigned int sched;
pthread_cond_t qempty;
pthread_cond_t qfull;
pthread_mutex_t mutex;
clock_t pendtime;
clock_t cstarttime;
clock_t cendtime;
clock_t start_time;
clock_t now;

deque<Product *> products;

void  createProduct();
void *producer(void *);
void *consumer(void *);
int fib(int n);

#endif
