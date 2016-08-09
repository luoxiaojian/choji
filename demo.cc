#include "fraction.h"
#include "choji.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {
	if(argc<2) {
		cout<<"usage: ./demo <taskset>"<<endl;
		return 1;
	}	

	FILE *fin=fopen(argv[1], "r");
	choji cj(fin);
	cj.run();
//	cj.output();
	if(!cj.checkAlloc()) 
		cout<<"Alloc failed..."<<argv[1]<<endl;
//	cj.outputAlloc();
	cj.generateSchedule();
	if(!cj.checkSchedule())
		cout<<"Sched failed..."<<argv[1]<<endl;
//	cj.outputSched();

	return 0;
}
