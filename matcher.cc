#include "matcher.h"
#include <cstring>
#include <cassert>

matcher::matcher(int m_, int n_) {
	m=m_;
	n=n_;
	unum=0;

	memset(linker, -1, sizeof(linker));
	memset(match, -1, sizeof(match));
	memset(uvis, false, sizeof(uvis));
	memset(g, 0, sizeof(g));
	memset(urgent, -1, sizeof(urgent));
}

void matcher::loadGraph(vector<fraction> *alloc) {
	for(int i=0; i<m; i++) {
		for(int j=0; j<n; j++) {
			if(alloc[j][i]>0) 
				g[i][j]=1;
			else
				g[i][j]=0;
		}
	}	
}

void matcher::loadUrgent(const set<int>& urg) {
	unum=urg.size();
	memset(urgent, -1, sizeof(urgent));
	int i=0;
	for(set<int>::iterator it=urg.begin(); it!=urg.end(); it++) {
		urgent[i]=*it;
		i++;
	}
}

bool matcher::dfs(int u) {
	for(int v=0; v<n; v++) {
		if(g[u][v] && !used[v]) {
			used[v]=true;
			if(linker[v]==-1 || dfs(linker[v])) {
				linker[v]=u;
				match[u]=v;
				return true;
			}
		}
	}
	return false;
}

bool matcher::isUrgent(int v) {
	for(int i=0; i<unum; i++) {
		if(urgent[i]==v)
			return true;
	}
	return false;
}

bool matcher::isMatched(int v) {
	for(int i=0; i<m; i++) {
		if(match[i]==v)
			return true;
	}
	return false;
}

bool matcher::dfsPath(int u) {
	int v=match[u];
	pathc.push_back(u);
	patht.push_back(v);
	uvis[u]=true;
	assert(v!=-1);
	if(isUrgent(v)) {
		for(int i=0; i<m; i++) {
			if(!uvis[i] && g[i][v]) {
				if(dfsPath(i))
					return true;
			}
		}
		pathc.pop_back();
		patht.pop_back();
		return false;
	} else {
		return true;
	}
}

void matcher::matchUrgent() {
	cout<<"------------------------------------"<<endl;
	for(int i=0; i<m; i++) {
		cout<<"p"<<i<<":\t";
		for(int j=0; j<n; j++) {
			if(g[i][j]==1)
				cout<<j<<" ";
		}
		cout<<endl;
	}
	cout<<"urgent: ";
	for(int i=0; i<unum; i++)
		cout<<urgent[i]<<" ";
	cout<<endl;
	for(int i=0; i<unum; i++) {
		if(isMatched(urgent[i]))
			continue;
		int cur=urgent[i];
		int flag=0;
		for(int j=0; j<m; j++) {
			if(!g[j][cur])
				continue;
			if(dfsPath(j)) {
				flag=1;
				int pathLen=pathc.size();
				match[j]=cur;
				for(int k=1; k<pathLen; k++)
					match[pathc[k]]=patht[k-1];
				pathc.clear();
				patht.clear();
				break;
			}
		}
		assert(flag);
	}
	for(int i=0; i<unum; i++)
		assert(isMatched(urgent[i]));
}

void matcher::hungary(int *sched) {
	int count=0;
	memset(linker, -1, sizeof(linker));
	memset(match, -1, sizeof(match));
	memset(uvis, false, sizeof(uvis));
	for(int u=0; u<m; u++) {
		memset(used, false, sizeof(used));
		if(dfs(u))
			count++;
	}
//	for(int i=0; i<m; i++)
//		cout<<"p"<<i<<"->t"<<match[i]<<", ";
//	cout<<endl;
	assert(m==count);
	matchUrgent();
//	for(int i=0; i<m; i++)
//		cout<<"p"<<i<<"->t"<<match[i]<<", ";
//	cout<<endl;
	for(int i=0; i<m; i++)
		sched[i]=match[i];
}
