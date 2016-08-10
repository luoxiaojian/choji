
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
		void outputAlloc();
		bool checkAlloc();
		void generateSchedule();
		bool checkSchedule();
		void outputSched();
		void getStat(int *migration, int *preemption);
		void getMStat(int *migration, int *preemption);

	private:
		void processACore(int ind);
		void postProcess();
		void mPostProcess();
		fraction most_to_reduce(int ind);

		vector<fraction> speed;
		vector<fraction> ut;
		vector<fraction> trt, crt, tru;
		vector<fraction> *alloc;
		vector<fraction> laxity;

		vector<int> execute, period;

		int m, n;
		vector<unit> *sched;
		vector<unit> *result;
		vector<unit> *mresult;
};

#endif
