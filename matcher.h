
#ifndef _MATCHER_H_
#define _MATCHER_H_

#include "fraction.h"
#include <set>
#include <vector>

using std::set;
using std::vector;

#define MAXN	60
#define MAXM	30

class matcher {
	public:
		explicit matcher(int m_, int n_);
		void loadGraph(fraction **alloc);
		void loadUrgent(const set<int>& urg);
		void hungary(int *sched);

	private:
		int m, n, unum;
		int g[MAXM][MAXN];
		int linker[MAXN];
		bool used[MAXN];
		int match[MAXM];
		int urgent[MAXN];
		bool uvis[MAXM];

		vector<int> pathc, patht;

		bool dfs(int x);
		bool isMatched(int v);
		bool isUrgent(int v);
		void matchUrgent();

		bool dfsPath(int u);
};

#endif
