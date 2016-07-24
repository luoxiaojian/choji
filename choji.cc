#include "choji.h"

bool operator<(const task& t1, const task& t2) {
	return t1.u<t2.u;
}

const fraction& task::getU() {
	return fraction;
}

int task::getId() {
	return id;
}

bool taskCluster::addATask(const task& t) {
	fraction newu=t.u+u;
	if(newu>maxu)
		return false;
	if(tasks.count(t.getId()))
		return false;

}

choji::choji(const char* fname) {
	FILE *fin=fopen(fname, "r");
	fscanf(fin, "%d%d%d", &tnum, &pnum, &lcm_period);

	int *execute=(int*)malloc(sizeof(int)*tnum);
	int *period=(int*)malloc(sizeof(int)*tnum);

	for(int i=0; i<tnum; i++)
		fscanf(fin, "%d", &execute[i]);
	for(int i=0; i<tnum; i++)
		fscanf(fin, "%d", &period[i]);

	for(int i=0; i<tnum; i++)
		tlist.push_back(task(i, execute[i], period[i]));

	sort(tlist.begin(), tlist.end());
}
