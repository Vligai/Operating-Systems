#include "assign.h"
/*
Vladislav Ligai
The goal of this assignment is to simulate a virtual memory management system.
*/
/*To string function*/
string to_String(int i)
{
    stringstream pstream;
    pstream << i;
    return pstream.str();
}
/*Load pages*/
void ld_p(int i, int j, int my_time)
{
	pproc[i][j].l_time=my_time;
	pproc[i][j].v_bit=1;
	pproc[i][j].u_bit=1;
	m_uid.push_back(pproc[i][j].uid);
	mem.push_back(pproc[i][j]);
}
void ld_p1(int k, int x, int my_time, int my_ind)
{
	pproc[k][x].l_time=my_time;
	pproc[k][x].v_bit=1;
	pproc[k][x].u_bit=1;
	m_uid[my_ind]=pproc[k][x].uid;
	mem[my_ind]=pproc[k][x];
}
/*split*/
tupl my_split(string tmp3)
{
	int tmp2;
    int i;
	int isplit;
    int tmp;
	string stmp="";
    string stmp2="";
        for(i=0;i<tmp3.length();i++)
        {
            if(tmp3[i]==':')
            {
                isplit=i;
                break;
            }
        }
        for(i=0;i<isplit;i++)
            stmp+=tmp3[i];
        for(i=isplit+1;i<tmp3.length();i++)
            stmp2+=tmp3[i];
        tmp=atoi(stmp.c_str());
    	tmp2=atoi(stmp2.c_str());
    	tupl a;
    	a.pid = tmp;
    	a.mem = tmp2;
    	return a;
}
/*find min*/
int findmin(vector<int> Clocktime)
{
	int i;
    int imin=0;
    int xmin=Clocktime[0];
	for(i=1;i<Clocktime.size();i++){
    		if(xmin==0){
    			break;
    		}
    		if(Clocktime[i]<xmin){
    			xmin=Clocktime[i];
    			imin=i;
    		}
    	}
    return imin;
}
/*getting ptrace file*/
vector<tupl> r_ptrace(string pname)
{
	int tmp;
	int i=0;
	int i2=0;
	int tmp2;
	int num;
	string line;
	vector<tupl> ptrace;
	vector<tupl> tmpv;
 	ifstream pfile (pname.c_str());
	tupl a;
 	if (pfile.is_open())
 	{
    		while ( getline (pfile,line) )
    		{
			string stmp="";
			string stmp2="";
			for (i=0;i<line.length();i++)
			{
				if(line[i]==' ')
				{
					num=i;
					break;
				}
			}
			for (i=0;i<num;i++)
				stmp=stmp+line[i];
			for (i=num+1;i<line.length();i++)
				stmp2=stmp2+line[i];
			tmp=atoi(stmp.c_str());
			tmp2=atoi(stmp2.c_str());
			a.pid=tmp;
			a.mem=tmp2;
			ptrace.push_back(a);
    		}
    		pfile.close();
  	}
	else
	{
		cout<<"File "<<pname<<" is not found"<<endl;
		return tmpv;
	}
	return ptrace;
}
/*getting plist file*/
vector<int> get_plist(string pname)
{
	int i2=0;
	int tmp2;
	int num=0;
	int tmp;
	int i=0;
	int j=0;
	string line;
	string inputs;
	vector<int> plist;
	vector<int> tmpv;
 	ifstream pfile (pname.c_str());
	if (pfile.is_open())
	{
    	while ( getline (pfile,line) )
    	{
    			string stmp="";
    			string stmp2="";
    			for (i=0;i<line.length();i++)
    			{
    				if(line[i]==' ')
    				{
    					num=i;
    					break;
    				}
    			}
    			for (i=0;i<num;i++)
    				stmp=stmp+line[i];
    			for (i=num+1;i<line.length();i++)
    				stmp2=stmp2+line[i];
    			tmp=atoi(stmp.c_str());
    			tmp2=atoi(stmp2.c_str());
    			plist.push_back(tmp2);
 		}
    	pfile.close();
	}
	else
	{
		cout<<"File "<<pname<<" is not found"<<endl;
		return tmpv;
	}
	return plist;
}
/*
Clock Based policy: use the simple version of this algorithm with only one use bit. This can be found in the text, or the slides.
First In, First Out (FIFO): the oldest page in memory will be the first to leave when a page swap is performed.
Least Recently Used: the page with the oldest access time will be replaced in memory when a page swap is performed.
*/
void page_swap(string alg, int apid, int amem, int my_time)
{
    int i=0;
    int isplit=0;
    int j=0;
    int tmp;
    int tmp2;
    string stmp="";
    string stmp2="";
    if(amem>=pproc[apid].size())
    	return;
    	/*FIFO alg*/
    if(alg=="FIFO")
    {
        string tmp3 = m_uid[0];
    	tupl a = my_split(tmp3);
    	pproc[a.pid][a.mem].v_bit=0;
    	if(m_uid.size()>0)
        	m_uid.erase(m_uid.begin());
        if (mem.size()>0)
        	mem.erase(mem.begin());
        ld_p(apid,amem,my_time);
    }
    /*Clock alg*/
    else if(alg=="Clock")
    {
		stmp="";
		stmp2="";
        string tmp3=m_uid[c_ptr];
    	tupl a = my_split(tmp3);
		tmp=a.pid;
		tmp2=a.mem;
		if(pproc[tmp][tmp2].u_bit)
		{
           pproc[tmp][tmp2].u_bit=0;
           mem[c_ptr].u_bit=0;
           c_ptr++;
           c_ptr%=m_uid.size();
           page_swap(alg,apid,amem,my_time);
           return;
        }
    	pproc[tmp][tmp2].v_bit=0;
        ld_p1(apid,amem,my_time,c_ptr);
        c_ptr++;
        c_ptr%=m_uid.size();
    }
    /*LRU alg*/
    else if(alg=="LRU")
    {
    	vector <int> Clocktime;
    	stmp="";
        stmp2="";
    	for(i=0;i<mem.size();i++)
    		Clocktime.push_back(mem[i].l_time);
    	c_ptr=findmin(Clocktime);
    	string tmp3=m_uid[c_ptr];
 		tupl a = my_split(tmp3);
    	pproc[a.pid][a.mem].v_bit=0;
        ld_p1(apid,amem,my_time,c_ptr);
    }
    else
    	cout << "Error: " <<alg << " does not exist" <<endl;
    return;
}
/*main*/
/*
P1: 'plist.txt' (given file)
P2: 'ptrace.txt' (given file)
P3: Size of pages, i.e., # of memory locations on each page.
P4: FIFO, LRU, or Clock for type of page replacement algorithms:
FIFO: First-in, First-Out
LRU: Least Recently Used
Clock: Clock Page Replacement
P5: flag to turn on/off pre-paging. If pre-paging is not turned on, we use demand paging by default.
+: turn it on
-: turn it off
*/
int main(int argc, char **argv)
{
	int p_page;
	int i;
	int j;
	int loc=512; /*512 memory locations*/
	int p_swap=0;
	char fl;
	string alg;
	string fplist;
	string fptrace;
	vector<int> plist;
	vector<tupl> ptrace;
	c_ptr=0;
	if (argc!=6)
	{
		cout<< "Usage: ./assign2 plist.txt ptrace.txt [size of Pages] [FIFO/LRU/Clock] [+/-]"<<endl;
		return 1;
	}
	fplist=argv[1];
	fptrace=argv[2];
	p_page=atoi(argv[3]);
	alg = argv[4];
	fl=argv[5][0];
	plist=get_plist(fplist);
	if(plist.empty())
		return 1;
	for (i=0;i<plist.size();i++)
	{
		vector<Page> v_tmp;
		int p_max = floor((double)plist[i]/p_page+.5);
		for (j=0;j<p_max;j++)
		{
			Page ptmp;
			ptmp.num_p=j;
			ptmp.v_bit=0;
			ptmp.u_bit=0;
			ptmp.l_time=0;
			ptmp.uid=to_String(i)+':'+to_String(j);
			v_tmp.push_back(ptmp);
		}
		pproc.push_back(v_tmp);
	}
	double page_ini=(double)(loc/p_page);
	page_ini=page_ini/pproc.size();
	int page_ini2=(int)page_ini;
	for(i=0;i<pproc.size();i++)
	{
		for(j=0;j<page_ini2;j++)
		{
			if(j<pproc[i].size())
				ld_p(i,j,0);
		}
	}
	int p_ext=512-page_ini2*pproc.size()*p_page;
	p_ext/=p_page;
	int q=361/p_page;
	if(alg=="FIFO")
		q-=10;
	if(alg=="LRU")
		q-=5;
	for(;p_ext>0;)
	{
		for(i=0;i<pproc.size();i++)
		{
			for(j=q;j<pproc[i].size();j++)
			{
				if(p_ext>0)
				{
					ld_p(i,j,0);
					p_ext--;
				}
				else
				{
					j=pproc[i].size();
					i=pproc.size();
				}
			}
		}
	}
	ptrace=r_ptrace(fptrace);
	if(ptrace.empty())
		return 1;
	for(i=0;i<ptrace.size();i++)
	{
		tupl a=ptrace[i];
		int memtmp=a.mem/p_page;
		string tuid=to_String(a.pid)+':'+to_String(memtmp);
		if(find(m_uid.begin(), m_uid.end(), tuid) != m_uid.end()) 
		{
			for(j=0;j<m_uid.size();j++)
			{
				if(m_uid[j]==tuid)
					break;
			}
			if(alg=="CLOCK")
			{
			    pproc[a.pid][memtmp].u_bit=1;
			    mem[j].u_bit=1;
			}
			if(alg=="LRU")
			{
			    pproc[a.pid][memtmp].l_time=i;
			    mem[j].l_time=i;
			}
		}
		else 
		{
		    p_swap++;
		    page_swap(alg,a.pid, memtmp, i);
		    bool is_swap=1;
		    int counter=0;
		    /*Pre-paging*/
		    for(;fl=='+' && is_swap && counter<(m_uid.size()-1);)
		    {
		    	memtmp=(memtmp+1)%m_uid.size();
		    	string tuid2=to_String(a.pid)+':'+to_String(memtmp);
		    	if(find(m_uid.begin(), m_uid.end(), tuid2) == m_uid.end()) 
		    	{
			    	page_swap(alg,a.pid, memtmp, i);
			    	is_swap=0;
		    	}
		    	counter++;
			}
		}
	}
	cout<<"Page Swaps: "<<p_swap<<endl;
	return 0;
}

