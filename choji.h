#ifndef _CHOJI_H_
#define _CHOJI_H_

#include "fraction.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <stdio.h>

using std::vector;
using std::sort;
using std::set;
using std::map;
using std::cout;
using std::endl;

class core {
	public:
		explicit core(int pid_, fraction s_)
			: pid(pid_),
			  s(s_) { }
		
		int pid;
		fraction s;
};

bool operator<(const core& c1, const core& c2);

class tinfo {
	public:
		explicit tinfo(int tid_, int e_, int p_)
			: tid(tid_),
			  e(e_),
			  p(p_),
			  u(e_, p_),
			  remaining(e_, p_) { }

		void addExec(const core& c, fraction duration);

		int tid;
		int e;
		int p;
		fraction u;
		map<int, fraction> alloc;
		fraction remaining;

};

class rtask {
	public:
		explicit rtask(const fraction& remaining);

		set<int> tids;
		set<int> pids;
		fraction u;
		fraction s;
};

bool operator<(const tinfo& t1, const tinfo& t2);

class choji {
	public:
		explicit choji(FILE *fin);
		bool schedulable() const;
		void run();
		bool checkSchedule();
		void output();

		int n, m;
		vector<tinfo> tlist;
		vector<core> clist;
		vector<rtask> rtlist;
	private:
		void getRTList();
		fraction getInterval() const;
		void updateTList(const fraction& interval);
		int pid2ind(int pid) const;
		int tid2ind(int tid) const;
};

#endif
