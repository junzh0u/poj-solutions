#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

struct Edge{
	int left,right,length;
};

bool lessByLength(const Edge &left,const Edge &right){
	return left.length<right.length;
}

const int VERTEX_NUM_MAX=1000,EDGE_NUM_MAX=15000;

int vertexNum,edgeNum,father[VERTEX_NUM_MAX],edgeUsedNum,edgeUsed[EDGE_NUM_MAX];
Edge edges[EDGE_NUM_MAX];

int root(int vertex){
	if(father[vertex]==vertex)
		return vertex;
	return father[vertex]=root(father[vertex]);
}

void connect(int left,int right){
	father[root(left)]=root(right);
}

int main(){
	scanf("%d%d",&vertexNum,&edgeNum);
	for(int e=0;e<edgeNum;e++){
		scanf("%d%d%d",&edges[e].left,&edges[e].right,&edges[e].length);
		edges[e].left--;
		edges[e].right--;
	}
	std::sort(edges,edges+edgeNum,lessByLength);
	for(int v=0;v<vertexNum;v++)
		father[v]=v;
	edgeUsedNum=0;
	for(int e=0;e<edgeNum;e++){
		if(root(edges[e].left)==root(edges[e].right))
			continue;
		edgeUsed[edgeUsedNum++]=e;
		connect(edges[e].left,edges[e].right);
	}
	printf("%d\n",edges[edgeUsed[edgeUsedNum-1]].length);
	printf("%d\n",edgeUsedNum);
	for(int i=0;i<edgeUsedNum;i++)
		printf("%d %d\n",edges[edgeUsed[i]].left+1,edges[edgeUsed[i]].right+1);
	return 0;
}