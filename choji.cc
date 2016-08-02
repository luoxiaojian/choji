#include "choji.h"
#include "fraction.h"

#define MIN(a, b) ((a)>(b)?(b):(a))

static bool greater(const fraction& f1, const fraction& f2) {
	return f1>f2;
}

choji::choji(FILE *fin) {
	fscanf(fin, "%d%d", &n, &m);
	speed=new fraction[m];
	ut=new fraction[n];
	trt=new fraction[n];
	crt=new fraction[m];
	execute=new int[n];
	period=new int[n];
	alloc=new fraction*[n];
	for(int i=0; i<n; i++) {
		int e, p;
		fscanf(fin, "%d%d", &e, &p);
		ut[i]=fraction(e, p);
		execute[i]=e;
		period[i]=p;
		trt[i]=1;
		tru[i]=ut[i];
	}
	for(int i=0; i<m; i++) {
		int a, b;
		fscanf(fin, "%d%d", &a, &b);
		speed[i]=fraction(a, b);
		crt[i]=1;
	}
	for(int i=0; i<n; i++)
		alloc[i]=new fraction[m];
	for(int i=0; i<n; i++) 
		for(int j=0; j<m; j++) 
			alloc[i][j]=0;
	
	sched=new vector<unit>[m];
	sort(&speed[0], &speed[m], greater);
}

void choji::run() {
	for(int i=0; i<m; i++)
		processACore(i);
}

void choji::output() {
	for(int i=0; i<m; i++) {
		cout<<"P"<<i<<":\t";
		for(int j=0; j<n; j++) 
			cout<<"( t"<<j<<", "<<alloc[j][i].strval()<<")\t";
		cout<<endl;
	}
}

fraction choji::most_to_reduce(int ind) {
	fraction sum=0;
	fraction diff=speed[ind+1]-speed[ind];
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
		if(!(su==ut[i]))
			return false;
		if(!(st>1))
			return false;
	}
	for(int i=0; i<m; i++) {
		fraction st=0;	
		for(int j=0; j<n; j++) {
			st=st+alloc[j][i];
		}
		if(!(st==1))
			return false;
	}
	return true;
}

static bool urgent(const tstate& t1, const tstate& t2) {
	return t1.laxity<t2.laxity;
}

void choji::generateSchedule() {
	struct tstate *state=(struct tstate*)malloc(sizeof(struct tstate)*n);		
	for(int i=0; i<n; i++) {
		state[i].tid=i;
		state[i].alloc=new fraction[m];
		for(int j=0; j<m; j++)
			state[i].alloc[j]=alloc[i][j];
		state[i].remaining=ut[i];
		state[i].laxity=1-ut[i];
	}
	fraction cur=0;
	while(!(cur==1)) {
		sort(&state[0], &state[n], urgent);
		for
	}
}
