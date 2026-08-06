#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

struct Edge{
	int from,to;
};

bool edgeLess(const Edge &left,const Edge &right){
	return left.from<right.from;
}

const int NODE_NUM_MAX=20000;
const int EDGE_NUM_MAX=(NODE_NUM_MAX-1)*2;

int nodeNum,parent[NODE_NUM_MAX],subtreeSize[NODE_NUM_MAX],balance[NODE_NUM_MAX],edgeNum;
Edge edges[EDGE_NUM_MAX];
bool visited[NODE_NUM_MAX];
int stack[NODE_NUM_MAX],stackSize;

int findEdge(int from,int begin,int end){
	if(edges[begin].from==from)
		return begin;
	int mid=(begin+end)/2;
	if(edges[mid-1].from>=from)
		return findEdge(from,begin,mid);
	return findEdge(from,mid,end);
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%d",&nodeNum);
		edgeNum=2*(nodeNum-1);
		for(int i=0;i<edgeNum;i+=2){
			scanf("%d%d",&edges[i].from,&edges[i].to);
			edges[i].from--;
			edges[i].to--;
			edges[i+1].from=edges[i].to;
			edges[i+1].to=edges[i].from;
		}
		std::sort(edges,edges+edgeNum,edgeLess);
		parent[0]=0;
		stackSize=0;
		stack[stackSize++]=0;
		std::fill(visited,visited+nodeNum,false);
		while(stackSize){
			int current=stack[stackSize-1];
			if(visited[current]){
				subtreeSize[current]=1;
				balance[current]=0;
				for(int i=findEdge(current,0,edgeNum);i<edgeNum && edges[i].from==current;i++){
					int child=edges[i].to;
					if(child==parent[current])
						continue;
					subtreeSize[current]+=subtreeSize[child];
					balance[current]=std::max(balance[current],subtreeSize[child]);
				}
				balance[current]=std::max(balance[current],nodeNum-subtreeSize[current]);
				stackSize--;
			}
			else{
				for(int i=findEdge(current,0,edgeNum);i<edgeNum && edges[i].from==current;i++){
					int child=edges[i].to;
					if(child==parent[current])
						continue;
					parent[child]=current;
					stack[stackSize++]=child;
				}
				visited[current]=true;
			}
		}
		int *minBalanceNode=std::min_element(balance,balance+nodeNum);
		printf("%d %d\n",minBalanceNode-balance+1,*minBalanceNode);
	}
	return 0;
}