#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int UNDEF=-1;
const int TYPE_NUM_MAX=2000,TYPE_LEN=7;
int typeNum;
char types[TYPE_NUM_MAX][TYPE_LEN+1];
int edges[TYPE_NUM_MAX][TYPE_NUM_MAX];

bool inGraph[TYPE_NUM_MAX];
int distanceToGraph[TYPE_NUM_MAX];
int graphSize;

int getNearestNotInGraph(){
	int index=UNDEF;
	for(int i=0;i<typeNum;i++)
		if(!inGraph[i] && (index==UNDEF || distanceToGraph[index]>distanceToGraph[i]))
			index=i;
	return index;
}

void connect(int index){
	inGraph[index]=true;
	distanceToGraph[index]=0;
	for(int to=0;to<typeNum;to++)
		if(!inGraph[to] && distanceToGraph[to]>edges[index][to])
			distanceToGraph[to]=edges[index][to];
	graphSize++;
}

void initializeGraph(){
	std::fill(inGraph,inGraph+typeNum,false);
	std::fill(distanceToGraph,distanceToGraph+typeNum,INT_MAX);
	graphSize=0;
}

int prim(){
	initializeGraph();
	connect(0);
	int distanceSum=0;
	while(graphSize<typeNum){
		int index=getNearestNotInGraph();
		distanceSum+=distanceToGraph[index];
		connect(index);
	}
	return distanceSum;
}

int getDistance(char* typeFrom,char* typeTo){
	int distance=0;
	for(int i=0;i<TYPE_LEN;i++)
		if(typeFrom[i]!=typeTo[i])
			distance++;
	return distance;
}

void calculateEdges(){
	for(int from=0;from<typeNum;from++){
		edges[from][from]=0;
		for(int to=from+1;to<typeNum;to++)
			edges[from][to]=edges[to][from]=getDistance(types[from],types[to]);
	}
}

bool input(){
	scanf("%d",&typeNum);
	if(typeNum==0)
		return false;
	for(int i=0;i<typeNum;i++)
		scanf("%s",types+i);
}

int main(){
	while(input()){
		calculateEdges();
		int distanceSum=prim();
		printf("The highest possible quality is 1/%d.\n",distanceSum);
	}
	return 0;
}