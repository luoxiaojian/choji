
#ifndef _CHOJI_H_
#define _CHOJI_H_

#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include "fraction.h"

using std::map;
using std::vector;
using std::sort;
using std::cout;
using std::endl;

class task {
	public:
		explicit task(int tid_, int e_, int p_)
			: tid(tid_),
			  e(e_),
			  p(p_),
			  u(e_, p_),
			  rt(1),
			  ru(e_, p_) { }

		int tid;
		int e, p;
		fraction u;

		fraction rt, ru;
		map<int, fraction> alloc;
};

class core {
	public:
		explicit core(int pid_, const fraction& s_)
			: pid(pid_),
			  s(s_),
			  rt(1) { }

		int pid;
		fraction s;
		fraction rt;
		map<int, fraction> alloc;
};

class choji {
	public:
		explicit choji(FILE *fin);

		void run();
		void output();

		vector<task> tlist;
		vector<core> clist;
		int m, n;

	private:
		void processACore(int ind);
};

#endif
