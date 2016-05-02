#include "assignment3.h"

void set_size(tree<treen>::iterator it, int size)
{
    (*it).size += size;
    if((*it).name == "/")
        return;
    else
        set_size(--it, size);
}
bool alloc_bl(unsigned int num, treen* file)
{
	
    unsigned int count;
    list<block>::iterator it_bl;
	it_bl = disk->begin();
    block* temp, temp2;
	list<block>::iterator pos_bl, next_bl;
    while(num > 0 && it_bl != disk->end())
    {
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
                    {
                        file->baddr->push_back(count);
                    }

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
        ++it_bl;
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
tree<treen>::iterator srch_nd(tree<treen>::iterator pos, string name)
{
    tree<treen>::sibling_iterator it;
    it = file_sys.begin(pos);
    while(it != file_sys.end(pos))
    {
        if((*it).name == name)
        {
            return it;
        }

        it++;
    }
    return NULL;
}

bool free_bl(unsigned int num, treen* file)
{
    list<int>::iterator pos;
    list<block>::iterator diskpos;
    block* temp;
    block* temp1;
    block* temp2;
	list<block>::iterator pos_bl, next_bl;
    pos = file->baddr->begin();
    while(pos != file->baddr->end() && num > 0)
    {
        diskpos = disk->begin();
        for( ; diskpos != disk->end(); diskpos++)
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
        --num;
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
bool cmdreader(string cmd)
{
    tree<treen>::sibling_iterator it;
    tree<treen>::iterator it_fr;
    vector<string> ins;
    vector<string>::iterator ins_loc;
    list<block>::iterator it_d;
    treen* temp;
	tree<treen>::breadth_first_queued_iterator itb;
    int frag;
	tree<treen>::iterator itt;
    list<int>::const_iterator blocks;
	if(cmd.empty() || cmd.find_first_not_of(' ') == std::string::npos)
        return true;
    ins = my_strtok(cmd, " ");
    if(ins.front() == "cd")
    {
        if(ins.size() > 1)
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
                        cout << "Directory " << *ins_loc << " not found" << endl;
                        break;
                    }
                    else if((*it).baddr != NULL)
                    {
                        cout << "Can't cd to " << *ins_loc << ": It's a file" << endl;
                        it = NULL;
                        break;
                    }
                }
                ins_loc++;
            }

            if(it != NULL)
                cur_directory = it;
        }
        else
            cout << "Invalid arguments to `cd`" << endl;
    }
    else if(ins.front() == "cd..")
        cmdreader("cd ../");
    else if(ins.front() == "ls")
    {
        it = file_sys.begin(cur_directory);
        while(it != file_sys.end(cur_directory))
        {
            if((*it).baddr != NULL)
                cout << (*it).name << " ";
            else
                cout << (*it).name << "/ ";
            it++;
        }
        cout << endl;
    }
    else if(ins.front() == "mkdir")
    {
        if(ins.size() < 2)
            cout << "Invalid arguments for `mkdir`" << endl;
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
                cout << "Cannot create directory " << ins.at(1) << ": File exists" << endl;
        }
    }
    else if(ins.front() == "create")
    {
        if(ins.size() == 1)
            cout << "Invalid arguments for `create`" << endl;
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
    else if(ins.front() == "append")
    {
        if(ins.size() < 3)
            cout << "Invalid arguments for `append`" << endl;
        else
        {
            it = srch_nd(cur_directory, ins.at(1));
			int fsize=(*file_sys.begin()).size;
			fsize=(int)(fsize/1024);
            if(it == NULL)
                cout << "Cannot find file " << ins.at(1) << endl;
            else if(fsize + atoi(ins.at(2).c_str()) > Size_d)
                cout << "Cannot append " << ins.at(2) << " bytes: disk full" << endl;
            else if((*it).baddr == NULL)
                cout << "Cannot append to " << ins.at(1) << ": it is a directory" << endl;
            else
            {
                alloc_bl(atoi(ins.at(2).c_str()) / Size_b, &(*it));
                it->tdate = glcounter++;
                cout << it->size << endl;
                it->frag = 32 - ((it->size + atoi(ins.at(2).c_str())) % Size_b);
                set_size(it, atoi(ins.at(2).c_str()));
            }
        }
    }
    else if(ins.front() == "remove")
    {
        if(ins.size() < 3)
            cout << "Invalid arguments for `remove`" << endl;
        else
        {
            it = srch_nd(cur_directory, ins.at(1));
            if(it == NULL)
                cout << "Cannot find file " << ins.at(1) << endl;
            else if((*it).baddr == NULL)
                cout << "Cannot remove from " << ins.at(1) << ": it is a directory" << endl;
            else
            {
                if((*it).size < (unsigned int) atoi(ins.at(2).c_str()))
                    cout << "Cannot remove " << ins.at(2) << " bytes from file: its size is " << (*it).size << endl;
                else
                {
                    free_bl(atoi(ins.at(2).c_str()) / Size_b, &(*it));
                    it->tdate = glcounter++;
                    it->frag = 32 - ((it->size - atoi(ins.at(2).c_str())) % Size_b);
                    set_size(it, atoi(ins.at(2).c_str()) * -1);
                }
            }
        }
    }
    else if(ins.front() == "delete")
    {
        if(ins.size() < 2)
            cout << "Invalid arguments for `delete`" << endl;
        else
        {
            it = srch_nd(cur_directory, ins.at(1));
            if(it != NULL)
            {
                if((*it).baddr == NULL)
                {
                    if(file_sys.number_of_children(it) > 0)
                        cout << "Cannot delete " << (*it).name << ": Directory not empty" << endl;
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
                cout << ins.at(1) << ": Directory/File not found" << endl;
        }
    }
    else if(ins.front() == "exit")
        return false;
    else if(ins.front() == "dir")
    {
	   itb = file_sys.begin_breadth_first();

    while(itb != file_sys.end_breadth_first())
    {
        cout << itb->name;
        if(itb->baddr == NULL && itb->name != "/")
            cout << "/";
        cout << endl;
        ++itb;
    }
    }
    else if(ins.front() == "prfiles")
    {
    itt = file_sys.begin();
    while(itt != file_sys.end())
    {
        if(itt->baddr != NULL)
        {
            cout << (*itt).name << ": ";
            blocks = itt->baddr->begin();
			if (blocks != itt->baddr->end())
			{
            while(blocks != itt->baddr->end())
            {
				cout << endl;
                cout << *blocks << " block(s) used";
                advance(blocks, 1);
            }
			}
			else
				cout << *blocks << " block(s) used";
            cout << endl;
        }

        ++itt;
    }
    }
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
            cout << "frag: " << frag << " bytes" << endl;
        else
            cout << "frag: 0 bytes" << endl;
    }
    else
        cout << "Invalid cmd: " << cmd << endl;
    return true;
}
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
        while(parse_ins != ins.end())
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
                        cout << "Error!!" << endl;
                        exit(1);
                    }
                }
            }
            ++parse_ins;
        }
    }
}

void read_f(string fname){
	string line;
	vector<string> ins;
	int count=0;
	int i=0;
	int isave=0;
	int len=0;
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
    		while(parse_ins != ins.end())
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
                        node->frag = 32 - (len % Size_b);
                        bl_num = node->size / Size_b;
                        if(bl_num == 0 && node->size > 0)
                            bl_num = 1;
                        if(!alloc_bl(bl_num, node))
                        {
                            cerr << "Unable to allocate blocks, disk full" << endl;
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
                            cerr << "Unknown directory: " << *parse_ins << endl;
                            exit(1);
                        }
                    }
                }
    
                ++parse_ins;
            }
    }
}

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
	Size_b = atoi(argv[6]);
    Size_d = iargv8;
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