#include<cstdio>
#include<algorithm>

const int NODE_NUM_MAX=100+2,NONE=-1,QUEUE_SIZE=NODE_NUM_MAX+1;

int nodeNum,flow[NODE_NUM_MAX][NODE_NUM_MAX],source,target,prev[NODE_NUM_MAX];

bool findAugumentPath(){
	std::fill(prev,prev+nodeNum,NONE);
	prev[source]=source;
	int queue[QUEUE_SIZE],queueBegin=0,queueEnd=0;
	queue[queueEnd]=source;
	queueEnd=(queueEnd+1)%QUEUE_SIZE;
	while(queueBegin!=queueEnd){
		int current=queue[queueBegin];
		queueBegin=(queueBegin+1)%QUEUE_SIZE;
		for(int next=0;next<nodeNum;next++)
			if(prev[next]==NONE && flow[current][next]>0){
				prev[next]=current;
				if(next==target)
					return true;
				queue[queueEnd]=next;
				queueEnd=(queueEnd+1)%QUEUE_SIZE;
			}
	}
	return false;
}

int residualCapacity(){
	int capacity=INT_MAX;
	for(int from=prev[target],to=target;from!=to;to=from,from=prev[from])
		if(capacity>flow[from][to])
			capacity=flow[from][to];
	return capacity;
}

void flipPath(int capacity){
	for(int from=prev[target],to=target;from!=to;to=from,from=prev[from]){
		flow[from][to]-=capacity;
		flow[to][from]+=capacity;
	}
}

int edmondsKarp(){
	int maxFlow=0;
	while(findAugumentPath()){
		int capacity=residualCapacity();
		maxFlow+=capacity;
		flipPath(capacity);
		maxFlow=maxFlow;
	}
	return maxFlow;
}

int main(){
	int powerStationNum,consumerNum,lineNum;
	while(scanf("%d%d%d%d",&nodeNum,&powerStationNum,&consumerNum,&lineNum)!=EOF){
		source=nodeNum++;
		target=nodeNum++;
		memset(flow,0,sizeof(int)*nodeNum*NODE_NUM_MAX);
		for(int i=0;i<lineNum;i++){
			int from,to,transport;
			scanf(" (%d,%d)%d",&from,&to,&transport);
			flow[from][to]=transport;
		}
		for(int i=0;i<powerStationNum;i++){
			int id,product;
			scanf(" (%d)%d",&id,&product);
			flow[source][id]=product;
		}
		for(int i=0;i<consumerNum;i++){
			int id,consume;
			scanf(" (%d)%d",&id,&consume);
			flow[id][target]=consume;
		}
		printf("%d\n",edmondsKarp());
	}
	return 0;
}