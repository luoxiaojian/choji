
#ifndef _CHOJI_H_
#define _CHOJI_H_

#include <stdio.h>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include "fraction.h"
#include "matcher.h"

using std::vector;
using std::sort;
using std::cout;
using std::endl;
using std::set;

struct unit{
	unit(int id, fraction dur)
		: tid(id),
		  duration(dur) { }
	int tid;
	fraction duration;
};

class choji {
	public:
		explicit choji(FILE *fin);
		void run();
		void output();
		bool checkAlloc();
		void generateSchedule();
		bool checkSchedule();

	private:
		void processACore(int ind);
		fraction most_to_reduce(int ind);

		fraction *speed;
		fraction *ut;
		fraction *trt, *crt, *tru;
		fraction **alloc;

		fraction *laxity;
		int *execute, *period;
		int m, n;
		vector<unit> *sched;
};

#endif
