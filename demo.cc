#include "fraction.h"
#include "choji.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main() {
	FILE *fin=fopen("taskset", "r");
	choji cj(fin);		
	if(cj.schedulable())
		cout<<"Taskset is schedulable"<<endl;
	else {
		cout<<"Taskset is not schedulable"<<endl;
		return 0;
	}
	cj.run();
	if(cj.checkSchedule())
		cout<<"Schedule succeed"<<endl;
	else {
		cout<<"Schedule failed"<<endl;
		return 0;
	}
	cj.output();
	return 0;
}
