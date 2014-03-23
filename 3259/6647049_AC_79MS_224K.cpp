#include<cstdio>
#include<algorithm>

const int FIELD_NUM_MAX=500,PATH_NUM_MAX=2500,WORMHOLE_NUM_MAX=200,DISTANCE_MAX=100000000;

struct Edge{
	int from,to,weight;
};

bool bellmanFord(int vertexNum,int source,int edgeNum,Edge edges[]){
	int distances[FIELD_NUM_MAX];
	std::fill(distances,distances+vertexNum,DISTANCE_MAX);
	distances[source]=0;
	for(int i=0;i<vertexNum;i++){
		for(int edgeID=0;edgeID<edgeNum;edgeID++)
			if(distances[edges[edgeID].to]>distances[edges[edgeID].from]+edges[edgeID].weight)
				distances[edges[edgeID].to]=distances[edges[edgeID].from]+edges[edgeID].weight;
	}
	for(int edgeID=0;edgeID<edgeNum;edgeID++)
		if(distances[edges[edgeID].to]>distances[edges[edgeID].from]+edges[edgeID].weight)
			return false;
	return true;
}

int main(){
	int farmNum;
	scanf("%d",&farmNum);
	while(farmNum--){
		int fieldNum,pathNum,wormholeNum;
		scanf("%d%d%d",&fieldNum,&pathNum,&wormholeNum);
		int vertexNum=fieldNum,edgeNum=2*pathNum+wormholeNum;
		Edge edges[2*PATH_NUM_MAX+WORMHOLE_NUM_MAX];
		int edgeID=0;
		while(pathNum-->0){
			scanf("%d%d%d",&edges[edgeID].from,&edges[edgeID].to,&edges[edgeID].weight);
			/*
			edges[edgeID+1].from=edges[edgeID].to;
			edges[edgeID+1].to=edges[edgeID].from;
			edges[edgeID+1].weight=edges[edgeID].weight;
			*/
			edges[edgeID+1]=edges[edgeID];
			std::swap(edges[edgeID+1].from,edges[edgeID+1].to);
			edgeID+=2;
		}
		while(wormholeNum-->0){
			scanf("%d%d%d",&edges[edgeID].from,&edges[edgeID].to,&edges[edgeID].weight);
			edges[edgeID].weight*=-1;
			edgeID++;
		}
		for(edgeID=0;edgeID<edgeNum;edgeID++){
			edges[edgeID].from--;
			edges[edgeID].to--;
		}
		printf("%s\n",bellmanFord(vertexNum,0,edgeNum,edges)?"NO":"YES");
	}
	return 0;
}