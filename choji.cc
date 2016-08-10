#include "choji.h"
#include "fraction.h"

#define MIN(a, b) ((a)>(b)?(b):(a))

static bool greater(const fraction& f1, const fraction& f2) {
	return f1>f2;
}

choji::choji(FILE *fin) {
	fscanf(fin, "%d%d", &n, &m);
	alloc=new vector<fraction>[n];
	sched=new vector<unit>[m];
	result=new vector<unit>[m];
	mresult=new vector<unit>[m];
	for(int i=0; i<n; i++) {
		int e, p;
		fscanf(fin, "%d%d", &e, &p);
		ut.push_back(fraction(e, p));
		execute.push_back(e);
		period.push_back(p);
		trt.push_back(1);
		tru.push_back(ut[i]);
	}
	for(int i=0; i<m; i++) {
		int a, b;
		fscanf(fin, "%d%d", &a, &b);
		speed.push_back(fraction(a, b));
		crt.push_back(1);
	}
	speed.push_back(0);
	crt.push_back(1);
	for(int i=0; i<n; i++) 
		for(int j=0; j<m; j++) 
			alloc[i].push_back(0);
	
	sort(speed.begin(), speed.end(), greater);
}

void choji::run() {
	for(int i=0; i<m; i++)
		processACore(i);
}

void choji::outputAlloc() {
	for(int i=0; i<m; i++) {
		cout<<"P"<<i<<":\t";
		for(int j=0; j<n; j++) 
			cout<<"( t"<<j<<", "<<alloc[j][i].strval()<<")\t";
		cout<<endl;
	}
}

fraction choji::most_to_reduce(int ind) {
	fraction sum=0;
	fraction diff=speed[ind]-speed[ind+1];
	assert(!(diff==0));
	for(int i=ind-1; i>=0; i--) {
		fraction cur=crt[i]*(speed[i]-speed[ind+1])/diff;
		sum=sum+cur;
	}
	return sum;
}

void choji::processACore(int ind) {
	fraction sum_min_ut=0;
	vector<fraction> min_ut;
	fraction curs=speed[ind];
	fraction nexts=speed[ind+1];
	if(curs==nexts)
		return ;
	for(int i=0; i<n; i++) {
		fraction most_to_exe_later=nexts*trt[i];
		fraction time_to_alloc=0;
		if(most_to_exe_later<tru[i]) {
			time_to_alloc=(tru[i]-most_to_exe_later)/(curs-nexts);
			sum_min_ut=sum_min_ut+time_to_alloc;
		}
		min_ut.push_back(time_to_alloc);
	}
	if(sum_min_ut>1) {
		assert(ind!=0);
		fraction ttreduce=sum_min_ut-1;
		assert(!(ttreduce>most_to_reduce(ind)));
		fraction toBalance=(sum_min_ut-1)*curs;
		for(int i=0; i<n; i++) {
			if(min_ut[i]>0) {
				fraction toChange=MIN(ttreduce, min_ut[i]);
				for(int j=ind-1; j>=0; j--) {
					fraction curmost=crt[j]*(speed[j]-nexts)/(curs-nexts);
					fraction tc=MIN(toChange, curmost);
					fraction curToChange=tc*(curs-nexts)/(speed[j]-nexts);
					crt[j]=crt[j]-curToChange;
					alloc[i][j]=alloc[i][j]+curToChange;
					trt[i]=trt[i]-curToChange;
					tru[i]=tru[i]-curToChange*speed[j];
					assert(!(trt[i]<0));
					toChange=toChange-tc;
					toBalance=toBalance-tc*curs;
					min_ut[i]=min_ut[i]-tc;
					sum_min_ut=sum_min_ut-tc;
					if(toChange==0)
						break;
				}
				if(toBalance==0)
					break;
			}
		}
	} 
	assert(!(sum_min_ut>1));
	crt[ind]=1-sum_min_ut;
	for(int i=0; i<n; i++) {
		trt[i]=trt[i]-min_ut[i];
		assert(!(trt[i]<0));
		tru[i]=tru[i]-min_ut[i]*curs;
		alloc[i][ind]=min_ut[i];
	}
}

bool choji::checkAlloc() {
	for(int i=0; i<n; i++) {
		fraction su=0, st=0;
		for(int j=0; j<m; j++) {
			su=su+alloc[i][j]*speed[j];
			st=st+alloc[i][j];
		}
		if(!(su==ut[i])) {
			return false;
		}
		if(st>1) {
			return false;
		}
	}
	for(int i=0; i<m; i++) {
		fraction st=0;	
		for(int j=0; j<n; j++) {
			st=st+alloc[j][i];
		}
		if(!(st==1)) {
			return false;
		}
	}
	return true;
}

void choji::generateSchedule() {
	for(int i=0; i<n; i++) {
		fraction sum;
		for(int j=0; j<m; j++) {
			sum=sum+alloc[i][j];
		}
		laxity.push_back(1-sum);
	}
	fraction cur=0;
	while(!(cur==1)) {
		set<int> urgent;
		cout<<"cur="<<cur.strval()<<endl;
		outputAlloc();
		for(int i=0; i<n; i++) {
			if(laxity[i]==0) {
				fraction sum=0;
				for(int j=0; j<m; j++)
					sum=sum+alloc[i][j];
				if(!(sum==0))
					urgent.insert(i);
			}
		}
		cout<<"urgent tasks: ";
		for(set<int>::iterator it=urgent.begin(); it!=urgent.end(); it++) {
			cout<<*it<<" ";
		}
		cout<<endl;
		int *list=new int[m];
		matcher ma(m, n);
		ma.loadGraph(alloc);
		ma.loadUrgent(urgent);
		ma.hungary(list);
		cout<<"choosed tasks: ";
		for(int i=0; i<m; i++) {
			cout<<list[i]<<" ";
		}
		cout<<endl;
		fraction dur=1;
		for(int i=0; i<n; i++) {
			bool choosed=false;
			int pid=-1;
			for(int j=0; j<m; j++) {
				if(list[j]==i) {
					choosed=true;
					pid=j;
					break;
				}
			}
			if(choosed) {
				dur=MIN(dur, alloc[i][pid]);
			} else {
				dur=MIN(dur, laxity[i]);
			}
		}
		//cout<<"dur="<<dur.strval()<<endl;
		for(int i=0; i<n; i++) {
			bool choosed=false;
			int pid=-1;
			for(int j=0; j<m; j++) {
				if(list[j]==i) {
					choosed=true;
					pid=j;
					break;
				}
			}
			if(choosed) {
				//cout<<"choose task"<<i<<" on processor"<<pid<<" for "<<dur.strval()<<endl;
				sched[pid].push_back(unit(i, dur));
				alloc[i][pid]=alloc[i][pid]-dur;
			} else {
				laxity[i]=laxity[i]-dur;	
			}
		}
		cur=cur+dur;
	}
}

bool choji::checkSchedule() {
	for(int i=0; i<n; i++) {
		for(int j=0; j<m; j++) {
			if(!(alloc[i][j]==0))
				return false;
		}
	}
	int events=sched[0].size();
	for(int i=1; i<m; i++) {
		if(events!=sched[i].size()) {
			cout<<1<<endl;
			return false;
		}
	}
	for(int i=0; i<events; i++) {
		set<int> active;
		for(int j=0; j<m; j++) {
			active.insert(sched[j][i].tid);
		}
		if(active.size()!=m) {
			cout<<2<<endl;
			return false;
		}
	}
	vector<fraction> alloced(n);
	for(int i=0; i<n; i++) {
		alloced[i]=0;
	}
	for(int i=0; i<m; i++) {
		for(int j=0; j<events; j++) {
			int tid=sched[i][j].tid;
			fraction u=sched[i][j].duration*speed[i];
			alloced[tid]=alloced[tid]+u;
		}
	}
	for(int i=0; i<n; i++) {
		if(!(alloced[i]==ut[i])) {
			cout<<"alloced="<<alloced[i].strval()<<" while ut="<<ut[i].strval()<<endl;
			cout<<3<<endl;
			return false;
		}
	}
	return true;
}

void choji::outputSched() {
	int events=sched[0].size();
	for(int i=0; i<m; i++) {
		cout<<"p"<<i<<":\t";
		for(int j=0; j<events; j++) {
			cout<<"["<<sched[i][j].tid<<", "<<sched[i][j].duration.strval()<<"]\t";
		}
		cout<<endl;
	}
}

void choji::postProcess() {
	for(int i=0; i<m; i++) {
		for(vector<unit>::iterator it=sched[i].begin(); it!=sched[i].end(); it++) {
			result[i].push_back(*it);
		}
		vector<unit>::iterator cur=result[i].begin();
		while(cur!=result[i].end()) {
			vector<unit>::iterator next=cur+1;
			if(next!=result[i].end() && cur->tid==next->tid) {
				cur->duration=cur->duration+next->duration;
				result[i].erase(next);
			} else {
				cur=next;
			}
		}
	}
}

void choji::mPostProcess() {
	for(int i=0; i<m; i++) {
		for(vector<unit>::iterator it=sched[i].begin(); it!=sched[i].end(); it++)
			mresult[i].push_back(*it);
		for(vector<unit>::reverse_iterator rit=sched[i].rbegin(); rit!=sched[i].rend(); rit++)
			mresult[i].push_back(*rit);
		vector<unit>::iterator cur=mresult[i].begin();
		while(cur!=mresult[i].end()) {
			vector<unit>::iterator next=cur+1;
			if(next!=mresult[i].end() && cur->tid==next->tid) {
				cur->duration=cur->duration+next->duration;
				mresult[i].erase(next);
			} else {
				cur=next;
			}
		}
	}
}

struct exec{
	exec(int p, fraction b, fraction e)
		: pid(p),
		  begin(b),
		  end(e) { }
	int pid;
	fraction begin, end;
};

static bool earlier(const struct exec& e1, const struct exec& e2) {
	return !(e1.end>e2.begin);
}

void choji::getStat(int *migration, int *preemption) {
	vector<exec> *texec=new vector<exec>[n];
	for(int i=0; i<m; i++) {
		fraction cur=0;
		for(vector<unit>::iterator it=result[i].begin(); it!=result[i].end(); it++) {
			texec[it->tid].push_back(exec(i, cur, cur+it->duration));
			cur=cur+it->duration;
		}
	}
	for(int i=0; i<n; i++) {
		migration[i]=0;
		sort(texec[i].begin(), texec[i].end(), earlier);
		int events=texec[i].size();
		preemption[i]=events;
		for(int j=1; j<events; j++) {
			if(texec[i][j].pid!=texec[i][j-1].pid)
				migration[i]++;
		}
	}
}
void choji::getMStat(int *migration, int *preemption) {
	vector<exec> *texec=new vector<exec>[n];
	for(int i=0; i<m; i++) {
		fraction cur=0;
		for(vector<unit>::iterator it=result[i].begin(); it!=result[i].end(); it++) {
			texec[it->tid].push_back(exec(i, cur, cur+it->duration));
			cur=cur+it->duration;
		}
	}
	for(int i=0; i<n; i++) {
		migration[i]=0;
		sort(texec[i].begin(), texec[i].end(), earlier);
		int events=texec[i].size();
		preemption[i]=events/2;
		for(int j=1; j<events; j++) {
			if(texec[i][j].pid!=texec[i][j-1].pid)
				migration[i]++;
		}
		migration[i]=migration[i]/2;
	}
}
