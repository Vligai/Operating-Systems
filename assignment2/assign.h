/*
Vladislav Ligai
Header file
*/
#include <string>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <iterator> 
#include <vector>
#include <stdio.h>
#include <climits>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <iostream>
using namespace std;
vector<string> m_uid;
int c_ptr;
struct tupl
{
	int pid;
	int mem;
};
struct Page
{
	int num_p;
	int l_time;
	string uid;
	bool v_bit;
	bool u_bit;
};
vector< vector <Page> > pproc;
vector<Page> mem;