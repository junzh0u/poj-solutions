#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<algorithm>

const int UNDEF=-1;
const int PIGHOUSE_NUM_MAX=1100,CUSTOMER_NUM_MAX=110;
const int NODE_NUM_MAX=CUSTOMER_NUM_MAX+2;

struct Customer{
	int keyNum;
	int keyIds[PIGHOUSE_NUM_MAX];
	int pigNumToBuy;
};
struct VisitorSequence{
	int visitorNum;
	int visitorIds[CUSTOMER_NUM_MAX];
};

int pighouseNum,customerNum;
int initialPigNum[PIGHOUSE_NUM_MAX];
Customer customers[CUSTOMER_NUM_MAX];
VisitorSequence visitorSequences[PIGHOUSE_NUM_MAX];
int nodeNum,sour,dest;
int edgeWeight[NODE_NUM_MAX][NODE_NUM_MAX];

inline int min(int a,int b){
	return (a<b)?a:b;
}

int augmentPath(){
	int prev[NODE_NUM_MAX];
	std::fill(prev,prev+nodeNum,UNDEF);
	int queue[NODE_NUM_MAX*2],queueBegin=0,queueEnd=0;
	queue[queueEnd++]=sour;
	while(queueEnd>queueBegin){
		int node=queue[queueBegin++];
		for(int next=0;next<nodeNum;next++){
			if(prev[next]!=UNDEF || edgeWeight[node][next]<=0)
				continue;
			prev[next]=node;
			queue[queueEnd++]=next;
			if(next==dest)
				break;
		}
		if(prev[dest]!=UNDEF)
			break;
	}
	if(prev[dest]==UNDEF){
#ifdef DEBUG
		printf("# Can not find an augment path.\n");
#endif
		return UNDEF;
	}
	int path[NODE_NUM_MAX],pathLen=0;
	path[pathLen++]=dest;
	while(true){
		int node=prev[path[pathLen-1]];
		path[pathLen++]=node;
		if(node==sour)
			break;
	}
#ifdef DEBUG
	printf("# Found an augment path:\n\t");
	for(int nodeIndex=pathLen-1;nodeIndex>=0;nodeIndex--){
		if(path[nodeIndex]==sour)
			printf("SOUR");
		else if(path[nodeIndex]==dest)
			printf("DEST");
		else
			printf("%d",path[nodeIndex]-1);
		if(nodeIndex!=0)
			printf(" -> ");
	}
	printf("\n");
#endif
	int pathFlow=INT_MAX;
	for(int nodeIndex=0;nodeIndex<pathLen-1;nodeIndex++){
		int from=path[nodeIndex+1],to=path[nodeIndex];
		pathFlow=min(pathFlow,edgeWeight[from][to]);
	}
#ifdef DEBUG
	printf("\tWith flow: %d\n",pathFlow);
#endif
	for(int nodeIndex=0;nodeIndex<pathLen-1;nodeIndex++){
		int from=path[nodeIndex+1],to=path[nodeIndex];
		edgeWeight[from][to]-=pathFlow;
		edgeWeight[to][from]+=pathFlow;
	}
#ifdef DEBUG
	printf("# Weight of edges after flip:\n");
	printf("\tFROM\\TO\t");
	for(int to=0;to<nodeNum;to++){
		if(to==sour)
			printf("SOUR");
		else if(to==dest)
			printf("DEST");
		else
			printf("%d",to-1);
		printf("\t");
	}
	printf("\n");
	for(int from=0;from<nodeNum;from++){
		printf("\t");
		if(from==sour)
			printf("SOUR");
		else if(from==dest)
			printf("DEST");
		else
			printf("%d",from-1);
		printf("\t");
		for(int to=0;to<nodeNum;to++){
			int weight=edgeWeight[from][to];
			if(weight==0)
				printf("-");
			else if(weight==INT_MAX)
				printf("INF");
			else
				printf("%d",weight);
			printf("\t");
		}
		printf("\n");
	}
#endif
	return pathFlow;
}

int edmondsKarp(){
	int flow=0;
	while(true){
		int flowRemain=augmentPath();
		if(flowRemain==UNDEF)
			return flow;
		flow+=flowRemain;
	}
}

int main(){
	scanf("%d%d",&pighouseNum,&customerNum);
	for(int pighouseIndex=0;pighouseIndex<pighouseNum;pighouseIndex++)
		scanf("%d",initialPigNum+pighouseIndex);
	for(int customerIndex=0;customerIndex<customerNum;customerIndex++){
		scanf("%d",&customers[customerIndex].keyNum);
		for(int keyIndex=0;keyIndex<customers[customerIndex].keyNum;keyIndex++){
			scanf("%d",customers[customerIndex].keyIds+keyIndex);
			customers[customerIndex].keyIds[keyIndex]--;
		}
		scanf("%d",&customers[customerIndex].pigNumToBuy);
	}
#ifdef DEBUG
	printf("# Initial number of pigs in pighouse:\n");
	for(int pighouseIndex=0;pighouseIndex<pighouseNum;pighouseIndex++)
		printf("\t%d:\t%d\n",pighouseIndex,initialPigNum[pighouseIndex]);
	printf("# Customers:\n");
	for(int customerIndex=0;customerIndex<customerNum;customerIndex++){
		printf("\t%d:\t",customerIndex);
		printf("Has %d keys:\t",customers[customerIndex].keyNum);
		for(int keyIndex=0;keyIndex<customers[customerIndex].keyNum;keyIndex++)
			printf("%d, ",customers[customerIndex].keyIds[keyIndex]);
		printf("\n\t\tWanna buy %d pigs.\n",customers[customerIndex].pigNumToBuy);
	}
#endif
	for(int pighouseIndex=0;pighouseIndex<pighouseNum;pighouseIndex++)
		visitorSequences[pighouseIndex].visitorNum=0;
	for(int customerIndex=0;customerIndex<customerNum;customerIndex++){
		for(int keyIndex=0;keyIndex<customers[customerIndex].keyNum;keyIndex++){
			int pighouseIndex=customers[customerIndex].keyIds[keyIndex];
			visitorSequences[pighouseIndex].visitorIds[visitorSequences[pighouseIndex].visitorNum++]=customerIndex;
		}
	}
#ifdef DEBUG
	printf("# Visitor sequence of pighouse:\n");
	for(int pighouseIndex=0;pighouseIndex<pighouseNum;pighouseIndex++){
		printf("\t%d has %d vistors:\t",pighouseIndex,visitorSequences[pighouseIndex].visitorNum);
		for(int visitorIndex=0;visitorIndex<visitorSequences[pighouseIndex].visitorNum;visitorIndex++)
			printf("%d, ",visitorSequences[pighouseIndex].visitorIds[visitorIndex]);
		printf("\n");
	}
#endif
	nodeNum=customerNum+2;
	sour=0;
	dest=customerNum+1;
	for(int nodeIndex=0;nodeIndex<nodeNum;nodeIndex++)
		std::fill(edgeWeight[nodeIndex],edgeWeight[nodeIndex]+nodeNum,0);
	for(int pighouseIndex=0;pighouseIndex<pighouseNum;pighouseIndex++){
		if(visitorSequences[pighouseIndex].visitorNum<=0)
			continue;
		int from=sour,to=visitorSequences[pighouseIndex].visitorIds[0]+1;
		edgeWeight[from][to]+=initialPigNum[pighouseIndex];
		for(int visitorIndex=1;visitorIndex<visitorSequences[pighouseIndex].visitorNum;visitorIndex++){
			from=visitorSequences[pighouseIndex].visitorIds[visitorIndex-1]+1;
			to=visitorSequences[pighouseIndex].visitorIds[visitorIndex]+1;
			edgeWeight[from][to]=INT_MAX;
		}
	}
	for(int customerIndex=0;customerIndex<customerNum;customerIndex++){
		int from=customerIndex+1,to=dest;
		edgeWeight[from][to]=customers[customerIndex].pigNumToBuy;
	}
#ifdef DEBUG
	printf("# Weight of edges:\n");
	printf("\tFROM\\TO\t");
	for(int to=0;to<nodeNum;to++){
		if(to==sour)
			printf("SOUR");
		else if(to==dest)
			printf("DEST");
		else
			printf("%d",to-1);
		printf("\t");
	}
	printf("\n");
	for(int from=0;from<nodeNum;from++){
		printf("\t");
		if(from==sour)
			printf("SOUR");
		else if(from==dest)
			printf("DEST");
		else
			printf("%d",from-1);
		printf("\t");
		for(int to=0;to<nodeNum;to++){
			int weight=edgeWeight[from][to];
			if(weight==0)
				printf("-");
			else if(weight==INT_MAX)
				printf("INF");
			else
				printf("%d",weight);
			printf("\t");
		}
		printf("\n");
	}
#endif
	printf("%d\n",edmondsKarp());
	return 0;
}