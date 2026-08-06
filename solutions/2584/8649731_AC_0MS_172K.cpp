#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>
#include<cassert>
#include<algorithm>

const int WORD_LEN_MAX=10;
const int CONTESTANT_NUM_MAX=20;
const int SIZE_NUM=5;
const int NODE_NUM_MAX=SIZE_NUM+CONTESTANT_NUM_MAX+2;

struct Contestant{
	int smallest,biggest;
};

int contestantNum;
Contestant contestants[CONTESTANT_NUM_MAX];
int shirtNum[SIZE_NUM];

int letterToSize(char letter){
	char letters[SIZE_NUM+1]="SMLXT";
	for(int i=0;i<SIZE_NUM;i++)
		if(letters[i]==letter)
			return i;
	assert(true);
	return -1;
}

bool input(){
	char word[WORD_LEN_MAX+1];
	scanf("%s",word);
	if(!strcmp(word,"ENDOFINPUT"))
		return false;
	scanf("%d",&contestantNum);
	for(int i=0;i<contestantNum;i++){
		scanf("%s",word);
		contestants[i].smallest=letterToSize(word[0]);
		contestants[i].biggest=letterToSize(word[1]);
	}
	for(int i=0;i<SIZE_NUM;i++)
		scanf("%d",shirtNum+i);
	scanf("%s",word);
	return true;
}

int nodeNum;
int graph[NODE_NUM_MAX][NODE_NUM_MAX];
int source,destination;

int sizeIndex(int id){
	return 1+id;
}

int contestantIndex(int id){
	return 1+SIZE_NUM+id;
}

void constructGraph(){
	nodeNum=SIZE_NUM+contestantNum+2;
	source=0;
	destination=nodeNum-1;
	for(int from=0;from<nodeNum;from++)
		std::fill(graph[from],graph[from]+nodeNum,0);
	for(int sizeId=0;sizeId<SIZE_NUM;sizeId++){
		int size=sizeIndex(sizeId);
		graph[source][size]=shirtNum[sizeId];
	}
	for(int contestantId=0;contestantId<contestantNum;contestantId++){
		int contestant=contestantIndex(contestantId);
		for(int sizeId=contestants[contestantId].smallest;sizeId<=contestants[contestantId].biggest;sizeId++){
			int size=sizeIndex(sizeId);
			graph[size][contestant]=1;
		}
		graph[contestant][destination]=1;
	}
}

int augmentPathLen;
int augmentPath[NODE_NUM_MAX];

int augmentPathCapacity(){
	int prevs[NODE_NUM_MAX];
	std::fill(prevs,prevs+nodeNum,-1);
	int queue[NODE_NUM_MAX],begin=0,end=0;
	queue[end++]=source;
	prevs[source]=source;
	while(end>begin){
		int current=queue[begin++];
		if(current==destination)
			break;
		for(int next=0;next<nodeNum;next++){
			if(prevs[next]>=0)
				continue;
			if(graph[current][next]<=0)
				continue;
			prevs[next]=current;
			queue[end++]=next;
		}
	}
	augmentPathLen=0;
	if(prevs[destination]<0)
		return -1;
	int capacity=INT_MAX;
	augmentPath[augmentPathLen++]=destination;
	while(true){
		int current=augmentPath[augmentPathLen-1];
		if(current==source)
			break;
		int prev=prevs[current];
		if(capacity>graph[prev][current])
			capacity=graph[prev][current];
		augmentPath[augmentPathLen++]=prev;
	}
	std::reverse(augmentPath,augmentPath+augmentPathLen);
	return capacity;
}

void addAugmentPath(int capacity){
	for(int i=0;i+1<augmentPathLen;i++){
		int current=augmentPath[i],next=augmentPath[i+1];
		graph[current][next]-=capacity;
		graph[next][current]+=capacity;
	}
}

int maxFlow(){
	int flow=0;
	while(true){
		int capacity=augmentPathCapacity();
		if(capacity<0)
			break;
#ifdef _DEBUG
		printf("---Found a Augment path---\n");
		printf("Length: %d\n",augmentPathLen);
		printf("Capacity: %d\n",capacity);
		for(int i=0;i<augmentPathLen;i++)
			printf("%d, ",augmentPath[i]);
		printf("\n---Augment path End---\n");
#endif
		addAugmentPath(capacity);
#ifdef _DEBUG
		printf("---Graph Begin---\n");
		for(int from=0;from<nodeNum;from++){
			for(int to=0;to<nodeNum;to++)
				printf("%3d ",graph[from][to]);
			printf("\n");
		}
		printf("---Graph End---\n");
#endif
		flow+=capacity;
	}
	return flow;
}

int main(){
	while(input()){
		constructGraph();
#ifdef _DEBUG
		printf("---Graph Begin---\n");
		for(int from=0;from<nodeNum;from++){
			for(int to=0;to<nodeNum;to++)
				printf("%3d ",graph[from][to]);
			printf("\n");
		}
		printf("---Graph End---\n");
#endif
		int flow=maxFlow();
		if(flow==contestantNum)
			printf("T-shirts rock!\n");
		else
			printf("I'd rather not wear a shirt anyway...\n");
	}
	return 0;
}