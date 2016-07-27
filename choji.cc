#include "choji.h"
#include "fraction.h"

#define MIN(a, b) ((a)>(b)?(b):(a))

static bool greaterc(const core& c1, const core& c2) {
	return c1.s>c2.s;
}

choji::choji(FILE *fin) {
	fscanf(fin, "%d%d", &n, &m);
	for(int i=0; i<n; i++) {
		int e, p;
		fscanf(fin, "%d%d", &e, &p);
		tlist.push_back(task(i, e, p));
	}
	for(int i=0; i<m; i++) {
		int a, b;
		fscanf(fin, "%d%d", &a, &b);
		clist.push_back(core(i, fraction(a, b)));
	}
	clist.push_back(core(m, 0));
	sort(clist.begin(), clist.end(), greaterc);
}

void choji::run() {
	for(int i=0; i<m; i++)
		processACore(i);
}

void choji::output() {
	for(int i=0; i<m; i++) {
		cout<<"P"<<i<<":\t";
		for(int j=0; j<n; j++) 
			cout<<"( t"<<j<<", "<<clist[i].alloc[j].strval()<<")\t";
		cout<<endl;
	}
}

fraction choji::most_to_reduce(int ind) {
	fraction sum=0;
	fraction curs=clist[ind].s;
	fraction nexts=clist[ind+1].s;
	fraction diff=curs-nexts;
	assert(!(curs==nexts));
	for(int i=ind-1; i>=0; i--) {
		fraction cur=clist[i].rt*(clist[i].s-nexts)/diff;
		sum=sum+cur;
	}
	return sum;
}

void choji::processACore(int ind) {
	fraction sum_min_ut=0;
	vector<fraction> min_ut;
	fraction curs=clist[ind].s;
	fraction nexts=clist[ind+1].s;
	if(curs==nexts)
		return ;
	for(int i=0; i<n; i++) {
		fraction most_to_exe_later=nexts*tlist[i].rt;
		fraction time_to_alloc=0;
		if(most_to_exe_later<tlist[i].ru) {
			time_to_alloc=(tlist[i].ru-most_to_exe_later)/(curs-nexts);
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
					fraction curmost=clist[j].rt*(clist[j].s-nexts)/(curs-nexts);
					fraction tc=MIN(toChange, curmost);
					fraction curToChange=tc*(curs-nexts)/(clist[j].s-nexts);
					clist[j].rt=clist[j].rt-curToChange;
					clist[j].alloc[i]=clist[j].alloc[i]+curToChange;
					tlist[i].rt=tlist[i].rt-curToChange;
					tlist[i].alloc[j]=tlist[i].alloc[j]+curToChange;
					assert(!(tlist[i].rt<0));
					tlist[i].ru=tlist[i].ru-curToChange*clist[j].s;
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
	clist[ind].rt=1-sum_min_ut;
	for(int i=0; i<n; i++) {
		tlist[i].rt=tlist[i].rt-min_ut[i];
		assert(!(tlist[i].rt<0));
		tlist[i].ru=tlist[i].ru-min_ut[i]*curs;
		tlist[i].alloc[ind]=min_ut[i];
		clist[ind].alloc[i]=min_ut[i];
	}
}
