#include<cstdio>
#include<algorithm>

//const int FARM_NUM_MAX=1000,ROAD_NUM_MAX=100000;

struct Edge{
	int from,to,weight;
};

void bellmanFord(int vertexNum,int distances[],int edgeNum,Edge edges[],int source){
	std::fill(distances,distances+vertexNum,INT_MAX/2);
	distances[source]=0;
	for(int i=0;i<vertexNum;i++)
		for(int edgeID=0;edgeID<edgeNum;edgeID++)
			if(distances[edges[edgeID].to]>distances[edges[edgeID].from]+edges[edgeID].weight)
				distances[edges[edgeID].to]=distances[edges[edgeID].from]+edges[edgeID].weight;
}

int main(){
	int farmNum,roadNum,partyLocation;
	scanf("%d%d%d",&farmNum,&roadNum,&partyLocation);
	partyLocation--;
	//Edge roads[ROAD_NUM_MAX];
	Edge *roads=new Edge[roadNum];
	for(int roadID=0;roadID<roadNum;roadID++){
		scanf("%d%d%d",&roads[roadID].from,&roads[roadID].to,&roads[roadID].weight);
		roads[roadID].from--;
		roads[roadID].to--;
	}
	//int timeBack[FARM_NUM_MAX];
	int *timeBack=new int[farmNum];
	bellmanFord(farmNum,timeBack,roadNum,roads,partyLocation);
	for(int roadID=0;roadID<roadNum;roadID++)
		std::swap(roads[roadID].from,roads[roadID].to);
	//int timeForward[FARM_NUM_MAX];
	int *timeForward=new int[farmNum];
	bellmanFord(farmNum,timeForward,roadNum,roads,partyLocation);
	int timeMax=0;
	for(int farmID=0;farmID<farmNum;farmID++)
		if(timeMax<timeForward[farmID]+timeBack[farmID])
			timeMax=timeForward[farmID]+timeBack[farmID];
	printf("%d\n",timeMax);
	delete[] roads;
	delete[] timeBack;
	delete[] timeForward;
	return 0;
}