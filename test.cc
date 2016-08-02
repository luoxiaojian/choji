#include <stdio.h>
#include <string.h>
#include <vector>
#include <assert.h>

using std::vector;

const int MAXM=10;
const int MAXN=20;

int m, n;

int g[MAXM][MAXN];
int linker[MAXN];
bool used[MAXN];

int match[MAXM];

int urgent[MAXN];
int unum;
bool uvis[MAXM];

void init() {
	m=6;
	n=7;
	g[0][0]=1;
	g[0][1]=1;
	g[0][3]=1;
	g[1][1]=1;
	g[1][4]=1;
	g[2][0]=1;
	g[2][3]=1;
	g[2][6]=1;
	g[3][2]=1;
	g[3][3]=1;
	g[3][5]=1;
	g[4][3]=1;
	g[5][3]=1;
	unum=1;
	urgent[0]=5;
}

bool dfs(int u) {
	int v;
	for(v=0; v<n; v++) {
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

bool isMatched(int v) {
	for(int i=0; i<m; i++) {
		if(match[i]==v)
			return true;
	}	
	return false;
}

vector<int> pathc, patht;

bool isUrgent(int v) {
	for(int i=0; i<unum; i++) {
		if(urgent[i]==v)
			return true;
	}
	return false;
}

bool dfsPath(int u) {
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

void matchUrgent() {
	for(int i=0; i<unum; i++) {
		if(isMatched(urgent[i])) 
			continue;
		int cur=urgent[i];
		int flag=0;
		vector<int> clist, tlist;
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
			}
		}
		assert(flag);
	}
}

int hungary() {
	int res=0;
	int u;
	memset(linker, -1, sizeof(linker));
	memset(match, -1, sizeof(match));
	memset(uvis, false, sizeof(uvis));
	for(u=0; u<m; u++) {
		memset(used, 0, sizeof(used));
		if(dfs(u))
			res++;
	}
	matchUrgent();
	for(int i=0; i<m; i++) {
		if(match[i]!=-1)
			printf("%d-->%d\n", i, match[i]);
	}
	return res;
}

int main() {
	init();
	printf("%d\n", hungary());
	return 0;
}
