/*
Vladislav Ligai
Header file
*/
#include "trees/tree_util.hh"
#include <unistd.h>
#include <sstream>
#include <climits>
#include <fstream>
#include <string.h>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
typedef struct s_block {
  unsigned int upb;
  unsigned int lwb;
  bool used;
} block;

typedef struct tnode {
  string name;
    int frag;
  list<int>* baddr;
  unsigned int tdate;
  unsigned int size;
} treen;


void set_size(tree<treen>::iterator, int size);
bool alloc_bl(unsigned int num, treen* file);
bool free_bl(unsigned int num, treen* file);
tree<treen>::iterator srch_nd(tree<treen>::iterator, string name);
bool cmdreader(string cmd);
void read_f(string fname);
void read_d(string fname);
vector<string> my_strtok(string line,string delim);

tree<treen> file_sys;
tree<treen>::iterator cur_directory;
list<block>* disk;
unsigned int glcounter;
unsigned int Size_d;
unsigned int Size_b;
unsigned int bl_num;
