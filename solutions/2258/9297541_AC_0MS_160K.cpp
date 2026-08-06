#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int NODE_NUM_MAX=25,EDGE_NUM_MAX=25;
int nodeNum,edgeNum;
bool adjacent[NODE_NUM_MAX][NODE_NUM_MAX];
//int edges[EDGE_NUM_MAX][2];

int longestPath(int begin){
	int lenMax=0;
	for(int next=0;next<nodeNum;next++){
		if(adjacent[begin][next]){
			adjacent[begin][next]=adjacent[next][begin]=false;
			int len=longestPath(next)+1;
			if(lenMax<len)
				lenMax=len;
			adjacent[begin][next]=adjacent[next][begin]=true;
		}
	}
	return lenMax;
}

int main(){
	while(true){
		scanf("%d%d",&nodeNum,&edgeNum);
		if(!nodeNum && !edgeNum)
			return 0;
		for(int from=0;from<nodeNum;from++)
			for(int to=0;to<nodeNum;to++)
				adjacent[from][to]=false;
		for(int i=0;i<edgeNum;i++){
			int a,b;
			scanf("%d%d",&a,&b);
			adjacent[a][b]=adjacent[b][a]=true;
		}
		int lenMax=0;
		for(int begin=0;begin<nodeNum;begin++){
			int len=longestPath(begin);
			if(lenMax<len)
				lenMax=len;
		}
		printf("%d\n",lenMax);
	}
}