#include "assignment3.h"
tree<tree_node> filesystem;
tree<tree_node>::iterator workingDirectory;

list<block>* disk;

unsigned int counter;
unsigned int diskSize;
unsigned int blockSize;
unsigned int numBlocks;



int main(int argc, char** argv)
{
  if(argc != 8)
    {
      printf("-f [input files storing information on files]\n"
	   "-d [input files storing information on directories]\n"
	   "-s [disk size]\n"
	   "-b [block size]\n");
      exit(1);
    }
  
  
  return 1;
}
