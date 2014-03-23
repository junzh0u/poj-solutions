#include<cstdio>
#include<algorithm>

const int POT_SIZE_MAX=200;
int sizeA,sizeB,target;

const int OPERATION_TYPE_NUM=6,OPERATION_NAME_LEM_MAX=9;
const char OPERATION_NAME[OPERATION_TYPE_NUM][OPERATION_NAME_LEM_MAX+1]=
	{"FILL(1)","FILL(2)","DROP(1)","DROP(2)","POUR(1,2)","POUR(2,1)"};

struct Status{
	int waterA,waterB;
};

Status operate(Status status,int operationType){
	int transfer;
	switch(operationType){
	case 0:
		status.waterA=sizeA;
		break;
	case 1:
		status.waterB=sizeB;
		break;
	case 2:
		status.waterA=0;
		break;
	case 3:
		status.waterB=0;
		break;
	case 4:
		transfer=std::min(status.waterA,sizeB-status.waterB);
		status.waterA-=transfer;
		status.waterB+=transfer;
		break;
	case 5:
		transfer=std::min(sizeA-status.waterA,status.waterB);
		status.waterA+=transfer;
		status.waterB-=transfer;
		break;
	}
	return status;
}

Status BFSQueue[POT_SIZE_MAX*POT_SIZE_MAX];
int head=0,tail=0;
bool visited[POT_SIZE_MAX][POT_SIZE_MAX]={false};
int operation[POT_SIZE_MAX][POT_SIZE_MAX];
Status prev[POT_SIZE_MAX][POT_SIZE_MAX];
int stepCount[POT_SIZE_MAX][POT_SIZE_MAX];

bool BFSPush(const Status& status,const int operationType,const Status& prevStatus,const int step){
	if(visited[status.waterA][status.waterB])
		return false;
	visited[status.waterA][status.waterB]=true;
	operation[status.waterA][status.waterB]=operationType;
	prev[status.waterA][status.waterB]=prevStatus;
	stepCount[status.waterA][status.waterB]=step;
	BFSQueue[tail++]=status;
	return true;
}

bool BFSEmpty(){
	return head>=tail;
}

Status BFSPop(){
	return BFSQueue[head++];
}

void printSequence(const Status& status){
	int operationType=operation[status.waterA][status.waterB];
	if(operationType==OPERATION_TYPE_NUM)
		return;
	printSequence(prev[status.waterA][status.waterB]);
	printf("%s\n",OPERATION_NAME[operation[status.waterA][status.waterB]]);
}

int main(){
	scanf("%d%d%d",&sizeA,&sizeB,&target);
	Status begin;
	begin.waterA=begin.waterB=0;
	BFSPush(begin,OPERATION_TYPE_NUM,begin,0);
	bool possible=false;
	while(!BFSEmpty()){
		Status current=BFSPop();
		if(current.waterA==target || current.waterB==target){
			printf("%d\n",stepCount[current.waterA][current.waterB]);
			printSequence(current);
			possible=true;
			break;
		}
		for(int operationType=0;operationType<OPERATION_TYPE_NUM;operationType++){
			Status next=operate(current,operationType);
			BFSPush(next,operationType,current,stepCount[current.waterA][current.waterB]+1);
		}
	}
	if(!possible)
		printf("impossible\n");
	return 0;
}