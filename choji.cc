#include "choji.h"

static bool cgreater(const core& c1, const core& c2) {
	return c1.s>c2.s;
}

static bool tugreater(const tinfo& t1, const tinfo& t2) {
	return t1.u>t2.u;
}

static bool trgreater(const tinfo& t1, const tinfo& t2) {
	return t1.remaining>t2.remaining;
}

void tinfo::addExec(const core& c, fraction duration) {
	alloc[c.pid]=alloc[c.pid]+duration;
	remaining=remaining-duration*c.s;
}

rtask::rtask(const fraction& remaining) {
	u=remaining;
}

choji::choji(FILE *fin) {
	fscanf(fin, "%d%d", &n, &m);
	for(int i=0; i<n; i++) {
		int e, p;
		fscanf(fin, "%d%d", &e, &p);
		tlist.push_back(tinfo(i, e, p));
	}
	for(int i=0; i<m; i++){
		int tmp1, tmp2;
		fscanf(fin, "%d%d", &tmp1, &tmp2);
		clist.push_back(core(i, fraction(tmp1, tmp2)));
	}
	sort(clist.begin(), clist.end(), cgreater);
	sort(tlist.begin(), tlist.end(), tugreater);
}

bool choji::schedulable() const  {
	fraction su=0, ss=0;
	for(int i=0; i<m; i++) {
		su=su+tlist[i].u;
		ss=ss+clist[i].s;
		if(su>ss)
			return false;
	}
	for(int i=m; i<n; i++) {
		su=su+tlist[i].u;
	}
	if(su==ss)
		return true;
	return false;
}

void choji::getRTList() {
	sort(tlist.begin(), tlist.end(), trgreater);
	rtlist.clear();
	int ind=0;
	while(ind!=m) {
		fraction rem=tlist[ind].remaining;
		rtask rt(rem);
		int num=0;
		fraction sum=0, avg;
		while(ind!=m && tlist[ind].remaining==rem) {
			rt.tids.insert(tlist[ind].tid);
			rt.pids.insert(clist[ind].pid);
			sum=sum+clist[ind].s;
			num=num+1;
			ind=ind+1;
		}
		avg=sum/num;
		rt.s=avg;
		rtlist.push_back(rt);
	}
}

fraction choji::getInterval() const{
	vector<fraction> interval(rtlist.size());
	int rtnum=rtlist.size();
	for(int i=0; i<rtnum-1; i++) {
		if(rtlist[i].s==rtlist[i+1].s)
			interval[i]=0;
		else 
			interval[i]=(rtlist[i].u-rtlist[i+1].u)/(rtlist[i].s-rtlist[i+1].s);
	}
	if(n==m || tlist[m].u==0)
		interval[rtnum-1]=0;
	else
		interval[rtnum-1]=rtlist[rtnum-1].u/rtlist[rtnum-1].s;
	fraction res=interval[0];
	for(int i=1; i<rtnum; i++) {
		if(res==0)
			res=interval[i];
		else if(interval[i]>0 && interval[i]<res)
			res=interval[i];
	}
	return res;
}

int choji::pid2ind(int pid) const {
	assert(pid<m);
	for(int i=0; i<m; i++) {
		if(clist[i].pid==pid)
			return i;
	}
	return -1;
}

int choji::tid2ind(int tid) const {
	assert(tid<n);
	for(int i=0; i<n; i++) {
		if(tlist[i].tid==tid)
			return i;
	}
	return -1;
}

void choji::updateTList(const fraction& interval) {
	int rtnum=rtlist.size();
	for(int i=0; i<rtnum; i++) {
		assert(rtlist[i].tids.size()==rtlist[i].pids.size());
		int pnum=rtlist[i].pids.size();
		for(set<int>::iterator it=rtlist[i].tids.begin(); it!=rtlist[i].tids.end(); it++) {
			int tind=tid2ind(*it);
			for(set<int>::iterator ppid=rtlist[i].pids.begin(); ppid!=rtlist[i].pids.end(); ppid++) {
				int pind=pid2ind(*ppid);
				tlist[tind].addExec(clist[pind], interval/pnum);
			}
		}
	}
}

void choji::run() {
	fraction cur=0, interval;
	while(!(cur==1)) {
		getRTList();
#ifdef DEBUG
		cout<<"-------------------------------------------"<<endl;
		cout<<"cur="<<cur.strval()<<endl;
		cout<<"rtlist: "<<endl;
		for(vector<rtask>::iterator it=rtlist.begin(); it!=rtlist.end(); it++) {
			cout<<"tasks( ";
			for(set<int>::iterator itt=it->tids.begin(); itt!=it->tids.end(); itt++)
				cout<<*itt<<", ";
			cout<<")";
			cout<<"--->processors( ";
			for(set<int>::iterator itt=it->pids.begin(); itt!=it->pids.end(); itt++)
				cout<<*itt<<", ";
			cout<<")";
			cout<<it->u.strval()<<"--->"<<it->s.strval()<<endl;
		}
#endif
		interval=getInterval();
		if(interval==0) 
			interval=1-cur;
#ifdef DEBUG
		cout<<"Interval="<<interval.strval()<<endl;
#endif
		updateTList(interval);
		cur=cur+interval;
	}
}

bool choji::checkSchedule() {
	for(int i=0; i<m; i++) {
		fraction sum=0;
		for(int j=0; j<n; j++) 
			sum=sum+tlist[j].alloc[i];
		if(!(sum==1)) {
			cout<<"Time allocated to processor"<<i<<" not equal to 1"<<endl;
			return false;
		}
	}
	for(int i=0; i<n; i++) {
		fraction sum=0;
		for(int j=0; j<m; j++) {
			sum=sum+tlist[i].alloc[clist[j].pid]*clist[j].s;
		}
		if(!(sum==tlist[i].u)) {
			cout<<"Time allocated to task"<<tlist[i].tid<<" not equal to it's u"<<endl;
			return false;
		}
		if(!(tlist[i].remaining==0)) {
			cout<<"Remaining time of task"<<tlist[i].tid<<" not eqaul to 0"<<endl;
			return false;
		}
	}
	return true;
}

void choji::output() {
	for(int i=0; i<n; i++) {
		cout<<"task"<<tlist[i].tid<<":\t";
		for(int j=0; j<m; j++) 
			cout<<"("<<j<<" ,"<<tlist[i].alloc[j].strval()<<")\t";
		cout<<endl;
	}
}


