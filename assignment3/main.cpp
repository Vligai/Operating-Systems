#include "assignment3.h"
/*
  The goal of this assignment is to manage a file system by first
  instantiating a file system based on an existing directory structure, 
  and then manipulating the files in the file system.
*/
/*strtok implementation for vectors*/
vector<string> my_strtok(string line,string delim){
  char* sttok;
  vector<string> ins;
  sttok = strtok(strdup(line.c_str()),delim.c_str());
  while(sttok!=NULL)
    {
      ins.push_back(sttok);
      sttok = strtok(NULL, delim.c_str());
    }
  return ins;   
}
/*search nodes*/
tree<treen>::iterator srch_nd(tree<treen>::iterator pos, string name)
{
  tree<treen>::sibling_iterator it;
  it = file_sys.begin(pos);
  for (;it != file_sys.end(pos); it++)
    {
      if((*it).name == name)
	return it;
    }
  return NULL;
}
/*
  command parser:
  -cd [directory]- set specified directory as the current directory
  -cd..- set parent directory as current directory
  -ls - list all files and sub-directories in current directory
  -mkdir [name]- create a new directory in the current directory
  -create [name]- create a new file in the current directory
  -append [name] [bytes]-append a number of bytes to the file
  -remove [name] [bytes]-delete a number of bytes from the file
  -delete [name]- delete the file or directory
  -exit-de-allocate data structures and exit program
  -dir- print outdirectory tree in breadth-first order
  -prfiles- print out all file information
  -prdisk- print out disk space information
*/
bool cmdreader(string cmd)
{
  int frag;
  tree<treen>::sibling_iterator it;
  tree<treen>::iterator it_fr;
  vector<string> ins;
  vector<string>::iterator ins_loc;
  list<block>::iterator it_d;
  treen* temp;
  tree<treen>::breadth_first_queued_iterator itb;
  tree<treen>::iterator itt;
  list<int>::const_iterator blocks;
  ins = my_strtok(cmd, " ");
  if(cmd.empty() || cmd.find_first_not_of(' ') == string::npos)
    return true;
  if(ins.front() == "cd..")
    cmdreader("cd ../");
  else if(ins.front() == "ls")
    {
      it = file_sys.begin(cur_directory);
      for(;it != file_sys.end(cur_directory);it++)
	{
	  //check if its a directory or a file
	  if((*it).baddr != NULL)
	    cout << (*it).name << " ";
	  else
	    cout << (*it).name << "/ ";
        }
      cout << endl;
    }
  /*update the file's linked-list data structure Lfile; 
    and update the file's time-stamp. Note that additional 
    disk blocks may not need to be allocated if there's enough
    space in the last disk block allocated to the file.
  */
  else if(ins.front() == "append")
    {
      //check the number of args for append
      if(ins.size() < 3)
	cout << "usage: append [file] [bytes to append]" << endl;
        else
	  {
            it = srch_nd(cur_directory, ins.at(1));
	    int fsize=(*file_sys.begin()).size;
	    if((*it).baddr == NULL)
	      cout << "Cannot append to a directory" << endl;
            else if(it == NULL)
	      cout << "File doesn't exit"<< endl;
            else if((int)(fsize/1024) + atoi(ins.at(2).c_str()) > Size_d)
	      cout << "Disk is full, cannot append" << endl;
            else
	      {
                alloc_bl(atoi(ins.at(2).c_str()) / Size_b, &(*it));
                it->tdate = glcounter++;
		cout << "appended the file" << endl;
                it->frag = bl_num - ((it->size + atoi(ins.at(2).c_str())) % Size_b);
                set_size(it, atoi(ins.at(2).c_str()));
            }
	  }
    }
  /*
    prints the block addresses for each file in the directory tree.
  */
  else if(ins.front() == "prfiles")
    {
      itt = file_sys.begin();
      for(;itt != file_sys.end();++itt)
	{
	  if(itt->baddr != NULL)
	    {
	      cout << (*itt).name << ": " << endl;
	      cout << "Size: " << (*itt).size << endl;
	      cout << "Time Stamp: " << (*itt).tdate << endl;
	      blocks = itt->baddr->begin();
	      if (blocks != itt->baddr->end())
		{
		  cout << "Blocks used: ";
		  while(blocks != itt->baddr->end())
		    {
		      cout << *blocks << " ";
		      advance(blocks, 1);
		    }
		}
	      else
		cout << *blocks << " blocks used";
	      cout << endl;
	    }       
	}
    }
  /*
    print out the range of block addresses that are
    in use and the range that are free, ordered by block
    address. Print out the total amount of internal fragmentation
  */
  else if(ins.front() == "prdisk")
    {
      it_d = disk->begin();
      it_fr = file_sys.begin();
      frag = 0;
      while(it_d != disk->end())
        {
	  if((*it_d).used)
	    cout << "In use: ";
	  else
	    cout << "Free: ";
	  if(it_d->lwb != it_d->upb)
	    cout << (*it_d).lwb << "-" << (*it_d).upb;
	  else
	    cout << it_d->lwb;
	  cout << endl;
	  advance(it_d, 1);
        }
      while(it_fr != file_sys.end())
        {
	  frag +=(it_fr->frag);
	  advance(it_fr, 1);
        }
      if(frag>0)
	cout << "fragmentation: " << frag << " bytes" << endl;
      else
	cout << "fragmentation: 0 bytes" << endl;
    }
  /*remove the corresponding directory node in the tree G*/
  else if(ins.front() == "delete")
    {
      if(ins.size() < 2)
	cout << "usage: delete [name of file/directory]" << endl;
      else
        {
	  it = srch_nd(cur_directory, ins.at(1));
	  if(it != NULL)
            {
	      if((*it).baddr == NULL)
                {
		  if(file_sys.number_of_children(it) > 0)
		    cout <<"Directory is not empty" << endl;
		  else
		    file_sys.erase(it);
                }
	      else
                {
		  free_bl((*it).size / Size_b, &(*it));
		  set_size(it, (*it).size * -1);
		  file_sys.parent(it)->tdate = glcounter++;
		  file_sys.erase(it);
                }
            }
	  else
	    cout << "File/Directory not found" << endl;
        }
    }
  /*
    append a new node to the directory tree 
    G at the appropriate branch.
  */
  else if(ins.front() == "mkdir")
    {
      //checking number of args for mkdir
      if(ins.size() < 2)
	cout << "usage: mkdir [name of directory]" << endl;
        else
	  {
            it = srch_nd(cur_directory, ins.at(1));
            if(it == NULL)
	      {
                temp = new treen();
                temp->size = 0;
                temp->name = ins.at(1);
                temp->tdate = glcounter++;
                file_sys.append_child(cur_directory, *temp);
            }
            else
	      cout << "Directory with that name already exists" << endl;
	  }
    }
  /*
    instantiate a linked-list for the new file 
    add the file to the directory tree G as a new node;
    and update the time-stamp of the file.
  */
  else if(ins.front() == "create")
    {
      if(ins.size() == 1)
	cout << "usage: create [name of the file]" << endl;
      else
        {
	  temp = new treen();
	  temp->size = 0;
	  temp->name = ins.at(1);
	  temp->baddr = new list<int>();
	  temp->tdate = glcounter++;
	  file_sys.append_child(cur_directory, *temp);
        }
    }
  /*
    update file's linked-list Lfile by de-allocating 
    disk blocks as specified by the number of bytes to
    remove; update the file's time-stamp.
  */
  else if(ins.front() == "remove")
    {
      if(ins.size() < 3)
	cout << "usage: remove [file] [bytes to remove]" << endl;
      else
        {
	  it = srch_nd(cur_directory, ins.at(1));
	  if(it == NULL)
	    cout << "File not found" << endl;
	  else if((*it).baddr == NULL)
	    cout << "Cannot remove bytes from a directory" << endl;
	  else
            {
	      if((*it).size < (unsigned int) atoi(ins.at(2).c_str()))
		cout << "Error couldn't remove bytes" << endl;
	      else
                {
		  free_bl(atoi(ins.at(2).c_str()) / Size_b, &(*it));
		  it->tdate = glcounter++;
		  cout << "Removed from file" << endl;
		  it->frag = bl_num - ((it->size - atoi(ins.at(2).c_str())) % Size_b);
		  set_size(it, atoi(ins.at(2).c_str()) * -1);
                }
            }
        }
    }
  else if(ins.front() == "exit")
    return false;
  /*
    Print out the directory structure 
    in breadth-first order when the command dir is issued.
  */
  else if(ins.front() == "dir")
    {
      itb = file_sys.begin_breadth_first();
      
      for(;itb != file_sys.end_breadth_first();++itb)
	{
	  cout << itb->name;
	  if(itb->name != "/" && itb->baddr == NULL)
	    cout << "/";
	  cout << endl;
	}
   }
  else if(ins.front() == "cd")
    {
      //checking args
      if(ins.size() < 1)
	cout << "usage: cd [directory name]" << endl;
	else
	  {
            ins = my_strtok(ins.at(1), "/");
            ins_loc = ins.begin();
            it = cur_directory;
            while(ins_loc != ins.end())
	      {
                if(*ins_loc == "..")
		  {
                    if(it != file_sys.begin())
		      it = file_sys.parent(it);
		  }
                else
                {
		  it = srch_nd(it, *ins_loc);
		  if(it == NULL)
                    {
		      cout << "Directory not found!" << endl;
		      break;
                    }
		  else if((*it).baddr != NULL)
                    {
		      cout << "Directories only!" << endl;
		      it = NULL;
		      break;
                    }
                }
                ins_loc++;
	      }
            if(it != NULL)
	      cur_directory = it;
	  }          
    }
  else
    cout << "Invalid cmd: " << cmd << endl;
  return true;
}
/*File Parser used to parse dir_list.txt*/
void read_d(string fname){
  ostringstream output;
  string line;
  treen* node;
  unsigned int size, bl_num;
  vector<string> ins;
  vector<string>::iterator parse_ins;
  tree<treen>::iterator it;
  ifstream ifname_dir(fname.c_str());
  while(getline(ifname_dir, line))
    {
      it = file_sys.begin();
      ins = my_strtok(line, "/");
      parse_ins = ins.begin();
      for(;parse_ins != ins.end();++parse_ins)
	{
	  if(strcmp((*parse_ins).c_str(), ".") != 0)
            {
	      if(parse_ins + 1 == ins.end())
                {
		  node = new treen();
		  node->name = *parse_ins;
		  node->tdate = ++glcounter;
		  node->size = 0;
		  node->baddr = NULL;
		  file_sys.append_child(it, *node);
                }
	      else
                {
		  it = srch_nd(it, (*parse_ins));
		  if(it == NULL)
                    {
		      cout << "it is NULL" << endl;
		      exit(1);
                    }
                }
            }
        }
    }
}
/*allocate blocks*/
bool alloc_bl(unsigned int num, treen* file)
{
  unsigned int count;
  list<block>::iterator pos_bl, next_bl, it_bl;
  it_bl = disk->begin();
  block* temp, temp2;
  for (;num > 0 && it_bl != disk->end(); ++it_bl)
    {
	/*
	  if(it_bl->used == true)
	  {
	  cerr << "block is used" << endl;
	  exit(1);
	  }
	*/
      if(it_bl->used == false)
        {
	  if(it_bl->lwb == it_bl->upb)
	    {
	      it_bl->used = true;
	      file->baddr->push_back(it_bl->lwb);
	      --num;
            }
            else
	      {
                if((it_bl->upb - it_bl->lwb) < num)
                {
		  it_bl->used = true;
		  for(count = it_bl->lwb; count <= it_bl->upb; count++)
		    file->baddr->push_back(count);
                    num -= (it_bl->upb - it_bl->lwb);
                }
                else
		  {
                    temp = new block();
                    temp->lwb = it_bl->lwb;
                    temp->upb = it_bl->lwb + num - 1;
                    temp->used = true;
                    temp2 = *it_bl;
                    temp2.lwb = temp->upb + 1;
                    it_bl = disk->erase(it_bl);
                    disk->insert(it_bl, *temp);
                    disk->insert(it_bl, temp2);
                    for(count = temp->lwb; count <= temp->upb; count++)
		      file->baddr->push_back(count);
                    num = 0;
                    break;
                }
            }
        }   
    }
  pos_bl = disk->begin();
  next_bl = disk->begin();
  advance(next_bl, 1);
  while(pos_bl != disk->end())
    {
      if(pos_bl->used == next_bl->used)
        {
	  temp = new block();
	  temp->lwb = pos_bl->lwb;
	  temp->upb = next_bl->upb;
	  temp->used = pos_bl->used;
	  disk->insert(pos_bl, *temp);
	  next_bl = disk->erase(next_bl);
	  pos_bl = disk->erase(pos_bl);
	  --pos_bl;
        }
      else
        {
	  advance(pos_bl, 1);
	  advance(next_bl, 1);
        }
    }
  return (num == 0);
}
/*set Parent's size*/
void set_size(tree<treen>::iterator it, int size)
{
  (*it).size += size;
  if((*it).name == "/")
    return;
  else
    set_size(--it, size);
}
/*free blocks that can be allocated to the files*/
bool free_bl(unsigned int num, treen* file)
{
  block* temp;
  block* temp1;
  block* temp2;
  list<int>::iterator pos;
  list<block>::iterator pos_bl, next_bl, diskpos;
  pos = file->baddr->begin();
  for(;pos != file->baddr->end() && num > 0;--num)
    {
      diskpos = disk->begin();
      for(; diskpos != disk->end(); diskpos++)
	{
	  if((unsigned int) *pos >= diskpos->lwb && (unsigned int) *pos <= diskpos->upb)
	    break;
        }
      if(diskpos->lwb == diskpos->upb)
            diskpos->used = false;
      else
        {
	  temp = new block();
	  temp1 = new block();
	  temp2 = new block();
	  if((unsigned int) *pos == diskpos->lwb)
            {
	      temp->lwb = *pos;
	      temp->upb = *pos;
	      temp->used = false;
	      temp1->lwb = diskpos->lwb + 1;
	      temp1->upb = diskpos->upb;
	      temp1->used = true;
	      disk->insert(diskpos, *temp);
	      disk->insert(diskpos, *temp1);
            }
	  else if((unsigned int) *pos == diskpos->upb)
            {
	      temp->lwb = *pos;
	      temp->upb = *pos;
	      temp->used = false;
	      temp1->lwb = diskpos->lwb;
	      temp1->upb = diskpos->upb - 1;
	      temp1->used = true;
	      disk->insert(diskpos, *temp);
	      disk->insert(diskpos, *temp1);
            }
	  else
	    {
	      temp->lwb = diskpos->lwb;
	      temp->upb = *pos - 1;
	      temp->used = true;
	      temp1->lwb = *pos;
	      temp1->upb = *pos;
	      temp1->used = false;
	      temp2->lwb = *pos + 1;
	      temp2->upb = diskpos->upb;
	      temp2->used = true;
	      disk->insert(diskpos, *temp);
	      disk->insert(diskpos, *temp1);
	      disk->insert(diskpos, *temp2);
            }
            disk->erase(diskpos);
        }
      pos = file->baddr->erase(pos);
    }
  pos_bl = disk->begin();
  next_bl = disk->begin();
  advance(next_bl, 1);
  /*merging blocks*/
  while(pos_bl != disk->end())
    {
      if(pos_bl->used == next_bl->used)
        {
	  temp = new block();
	  temp->lwb = pos_bl->lwb;
	  temp->upb = next_bl->upb;
	  temp->used = pos_bl->used;
	  disk->insert(pos_bl, *temp);
	  next_bl = disk->erase(next_bl);
	  pos_bl = disk->erase(pos_bl);
	  --pos_bl;
        }
      else
        {
	  advance(pos_bl, 1);
	  advance(next_bl, 1);
        }
    }
    return 0;
}
/*File Parser used to parse file_list.txt*/
void read_f(string fname){
  int count=0;
  int i=0;
  int isave=0;
  int len=0;
  string line;
  vector<string> ins;
  ifstream ifname_file(fname.c_str());
  tree<treen>::iterator it;
  treen* node;
  vector<string>::iterator parse_ins;
  while(getline(ifname_file, line))
      {
	for(i=0;i<line.length();i++)
	  {
	    if(isspace(line[i]))
	      count++;
	    if(count==14)
	      {
		isave=i;
		break;
	      }
	  }
	for(;i<line.length();i++)
	  {
	    if(isspace(line[i]));
	    else
	      break;
	  }
	for(;i<line.length();i++)
	  {
	    if(isspace(line[i]))
	      break;
	  }
	int itemp=i-isave+1;
	string stemp=line.substr(isave+1,itemp);
	len=atoi( stemp.c_str() );
	int len2=len/1024;
	if(len2)
	  len2++;
	int itemp2=line.find("./");
	string stemp2=line.substr(itemp2+2,line.length()-(itemp2+2));
	ins = my_strtok(stemp2, "/");
	parse_ins = ins.begin();
	it = file_sys.begin();
	for(;parse_ins != ins.end();++parse_ins)
	  {
	    if(strcmp((*parse_ins).c_str(), ".") != 0)
	      {
		if(parse_ins + 1 == ins.end())
		  {
		    node = new treen();
		    node->name = *parse_ins;
		    node->tdate = ++glcounter;
		    node->size = len2;
		    node->baddr = new list<int>();
		    node->frag = bl_num - (len % Size_b);
		    bl_num = node->size / Size_b;
		    if(bl_num == 0 && node->size > 0)
		      bl_num = 1;
		    if(!alloc_bl(bl_num, node))
		      {
			cerr << "Disk Full" << endl;
			exit(1);
		      }
		    file_sys.append_child(it, *node);
		    set_size(it, len);
		  }
		else
		  {
		    it = srch_nd(it, *parse_ins);
		    if(it == NULL)
		      {
			cerr << "Unknown directory"<< endl;
			exit(1);
		      }
		  }
	      }
	  }
      }
}
/*
  main
  -f [input files storing information on files]
  -d [input files storing information on directories]
  -s [disk size]
  -b [block size]
*/
int main(int argc, char **argv){
  treen* root_dir;
  tree<treen>::iterator it;
  block* temp;
  string cmd;
  string argv2;
  string argv4;	
  vector<string> cur_dir;
  unsigned int i;
  if (argc!=9){
    cout<< "Usage: ./assign3 -d [input files storing information on files]";
    cout<< "-f [input files storing information on files]";
    cout<<"-s [disk size]";
    cout<<"-b [block size]"<<endl;
    cout << argc << endl;
    return 1;
  }
  argv2=argv[2];
  argv4=argv[4];
  int iargv6=atoi(argv[6]);
  int iargv8=atoi(argv[8]);
  Size_b = iargv8;
  Size_d = iargv6;
  bl_num = floor(Size_d / Size_b);
  temp = new block();
  temp->lwb = 0;
  temp->upb = bl_num - 1;
  temp->used = false;
  disk = new list<block>();
  disk->push_front(*temp);
  root_dir = new treen();
  root_dir->name = "/";
  root_dir->size = 0;
  root_dir->tdate = 0;
  root_dir->baddr = NULL;
  glcounter = 0;
  file_sys.set_head(*root_dir);
  cur_directory = file_sys.begin();
  if(!argv2.empty())
    read_d(argv2);
  
  if(!argv4.empty())
    read_f(argv4);
  
  while(1)
    {
      it = cur_directory;
      while(it != file_sys.begin())
	{
	  cur_dir.insert(cur_dir.begin(), (*it).name);
	  it = file_sys.parent(it);
	}
      for(i = 0; i < cur_dir.size(); i++)
        cout << cur_dir.at(i) << "/";
      cur_dir.clear();
      cout << "$> ";
      getline(cin, cmd);
      if(!cmdreader(cmd))
	break;
    }
  for(it = file_sys.begin(); it != file_sys.end(); it++)
    {
      if(it->baddr != NULL)
	it->baddr->clear();
    }
  file_sys.clear();
  disk->clear();
	printf("   	      __ \n"
"	   .-'  | ""\n"
" 	  /    <| ""\n"
"  	 /     \' ""\n"
"  	 |_.- o-o ""\n"
"  	 / C  -._)\\ "" \n"
"        /',        | ""\n"
"       |   `-,_,__,' ""\n"
"       (,,)====[_]=| ""\n"
"         '.   ____/ ""\n"
"           | -|-|_ ""\n"
"           |____)_) 	   de-allocated data structures Bye!\n");
    
    return 0;
}
