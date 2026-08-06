#include<cstdio>
#include<algorithm>

struct Interval{
	int first,last,count;
};

struct Edge{
	int from,to,weight;
};

bool edgeLess(const Edge& left,const Edge& right){
	if(left.from!=right.from)
		return left.from<right.from;
	return left.to<right.to;
}

const int INTERVAL_NUM_MAX=10000,RANGE=10000,QUEUE_SIZE=RANGE+3;

int intervalNum,edgeNum=0,min=INT_MAX,max=-INT_MAX,edgeIndex[RANGE+2],distance[RANGE+2],queue[QUEUE_SIZE];
Interval intervals[INTERVAL_NUM_MAX];
Edge edges[INTERVAL_NUM_MAX+2*(RANGE+1)];

int main(){
	scanf("%d",&intervalNum);
	for(int i=0;i<intervalNum;i++){
		scanf("%d%d",&intervals[i].first,&intervals[i].last);
		intervals[i].count=2;
		if(min>intervals[i].first-1)
			min=intervals[i].first-1;
		if(max<intervals[i].last+1)
			max=intervals[i].last+1;
		Edge edge={intervals[i].last+1,intervals[i].first,-intervals[i].count};
		edges[edgeNum++]=edge;
	}
	for(int i=min;i<=max;i++){
		Edge next={i,i+1,1},prev={i+1,i,0};
		edges[edgeNum++]=next;
		edges[edgeNum++]=prev;
	}
	std::sort(edges,edges+edgeNum,edgeLess);
	edgeIndex[edges[0].from]=0;
	int prev=edges[0].from;
	for(int i=1;i<edgeNum;i++)
		if(prev!=edges[i].from){
			prev=edges[i].from;
			edgeIndex[prev]=i;
		}
	std::fill(distance+min,distance+max+1,INT_MAX);
	int queueBegin=0,queueEnd=0;
	distance[max]=0;
	queue[queueEnd]=max;
	queueEnd=(queueEnd+1)%QUEUE_SIZE;
	while(queueEnd!=queueBegin){
		int from=queue[queueBegin];
		queueBegin=(queueBegin+1)%QUEUE_SIZE;
		for(int i=edgeIndex[from];i<edgeNum && edges[i].from==from;i++){
			int to=edges[i].to;
			if(distance[to]>distance[from]+edges[i].weight){
				distance[to]=distance[from]+edges[i].weight;
				queue[queueEnd]=to;
				queueEnd=(queueEnd+1)%QUEUE_SIZE;
			}
		}
	}
	printf("%d\n",-distance[min]);
	return 0;
}