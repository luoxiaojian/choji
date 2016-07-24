#include <stdio.h>
#include "fraction.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::sort;

vector<fraction> u, speed;
int on, om, nn, nm;

static bool greater(const fraction& f1, const fraction& f2) {
	return f1>f2;
}

void generateOne(int tid, int pnum) {
	fraction sum=0;
	for(int i=tid; i<on; i++)
		sum=sum+u[i];
	fraction avg=sum/pnum;
	if(u[tid]>avg) {
		speed.push_back(u[tid]);
		generateOne(tid+1, pnum-1);
	} else {
		for(int i=0; i<pnum; i++)
			speed.push_back(avg);
	}
}

int main(int argc, char **argv) {
	if(argc<3) {
		printf("./modifier <src_file> <tgt_file>\n");
		return 0;
	}

	FILE *fin, *fout;
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w+");

	int tmp;
	fscanf(fin, "%d%d%d", &on, &om, &tmp);
	nm=om/2*3;
	nn=on;

	int *e, *p;
	e=(int*)malloc(sizeof(int)*on);
	p=(int*)malloc(sizeof(int)*on);

	for(int i=0; i<on; i++)
		fscanf(fin, "%d", &p[i]);
	for(int i=0; i<on; i++)
		fscanf(fin, "%d", &e[i]);

	for(int i=0; i<on; i++)
		u.push_back(fraction(e[i], p[i]));
	sort(u.begin(), u.end(), greater);

	generateOne(0, nm);

	fprintf(fout, "%d %d\n", nn, nm);
	for(int i=0; i<nn; i++)
		fprintf(fout, "%d %d\n", u[i].nu_, u[i].de_);
	for(int i=0; i<nm; i++)
		fprintf(fout, "%d %d\n", speed[i].nu_, speed[i].de_);

	fflush(fout);
	fclose(fout);

	return 0;
}
