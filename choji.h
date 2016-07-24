#ifndef _CHOJI_H_
#define _CHOJI_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>
#include "fraction.h"

using std::vector;
using std::sort;
using std::set;

class task{
	public:
		explicit task(int id, int c, int t)
			: tid(id),
			  e(c),
			  p(t),
			  u(c, t) { }
		friend bool operator<(const task&, const task&);
		int getId() const;
		fraction getU() const;
	private:
		int tid, e, p;
		fraction u;
};

class taskCluster{
	public:
		explicit taskCluster(int id, const fraction& mu)
			: tcid(id),
			  maxu(mu) { }
		bool addATask(const task& t);
	private:
		int tcid;
		set<int> tasks;
		fraction u, maxu;
};

class processor{
	public:
		explicit processor(int id, const fraction& speed)
			: pid(id),
			  s(speed) { }
	private:
		int pid;
		fraction s;
};

bool operator<(const task&, const task&);

class choji{
	public:
		explicit choji(const char* fname);	

	private:
		int tnum, pnum, lcm_period;
		fraction capacity;
		vector<task> tlist;
		vector<taskCluster> tclist;
		vector<processor> plist;

		void generateProcessors();
		int generateAProcessor(int tid, int pid, fraction remaining);
};

#endif
