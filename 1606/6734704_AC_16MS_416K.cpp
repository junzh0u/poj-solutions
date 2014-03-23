#include<cstdio>
#include<memory>
#include<algorithm>

const int OPERATION_TYPE_NUM=6,OPERATION_NAME_LEM_MAX=9;
const char OPERATION_NAME[OPERATION_TYPE_NUM][OPERATION_NAME_LEM_MAX+1]=
	{"fill A","fill B","empty A","empty B","pour A B","pour B A"};

struct Status{
	int waterA,waterB;
};

class BFSQueue{
public:
	BFSQueue(int sizeA,int sizeB){
		this->sizeA=sizeA;
		this->sizeB=sizeB;
		queue=new Status[sizeA*sizeB];
		head=tail=0;
		visited=new bool*[sizeA+1];
		step=new int*[sizeA+1];
		operationFrom=new int*[sizeA+1];
		operationTo=new int*[sizeA+1];
		prev=new Status*[sizeA+1];
		next=new Status*[sizeA+1];
		for(int index=0;index<sizeA+1;index++){
			visited[index]=new bool[sizeB+1];
			std::fill(visited[index],visited[index]+sizeB+1,false);
			step[index]=new int[sizeB+1];
			operationFrom[index]=new int[sizeB+1];
			operationTo[index]=new int[sizeB+1];
			prev[index]=new Status[sizeB+1];
			next[index]=new Status[sizeB+1];
		}
		Status begin={0,0};
		visited[0][0]=true;
		step[0][0]=0;
		prev[0][0].waterA=-1;
		prev[0][0].waterB=-1;
		push(begin);
	}
	~BFSQueue(){
		for(int index=0;index<sizeA+1;index++){
			delete[] visited[index];
			delete[] step[index];
			delete[] operationFrom[index];
			delete[] operationTo[index];
			delete[] prev[index];
			delete[] next[index];
		}
		delete[] step;
		delete[] visited;
		delete[] operationFrom;
		delete[] operationTo;
		delete[] prev;
		delete[] next;
	}
	void push(const Status& status){
		queue[tail++]=status;
	}
	bool empty(){
		return head>=tail;
	}
	Status pop(){
		return queue[head++];
	}
	Status operate(Status status,int type){
		int transfer;
		switch(type){
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
	void revisePath(Status end){
		while(true){
			Status status=prev[end.waterA][end.waterB];
			if(status.waterA==-1)
				break;
			next[status.waterA][status.waterB]=end;
			operationTo[status.waterA][status.waterB]=operationFrom[end.waterA][end.waterB];
			end=status;
		}
	}
	void printPath(){
		Status begin={0,0};
		while(true){
			Status status=next[begin.waterA][begin.waterB];
			if(status.waterA==-1){
				printf("success\n");
				break;
			}
			printf("%s\n",OPERATION_NAME[operationTo[begin.waterA][begin.waterB]]);
			begin=status;
		}
	}
	bool bfs(int target){
		while(!empty()){
			Status current=pop();
			if(current.waterB==target){
				next[current.waterA][current.waterB].waterA=-1;
				next[current.waterA][current.waterB].waterB=-1;
				revisePath(current);
				printPath();
				return true;
			}
			for(int operationType=0;operationType<OPERATION_TYPE_NUM;operationType++){
				Status next=operate(current,operationType);
				if(visited[next.waterA][next.waterB])
					continue;
				visited[next.waterA][next.waterB]=true;
				step[next.waterA][next.waterB]=step[current.waterA][current.waterB]+1;
				operationFrom[next.waterA][next.waterB]=operationType;
				prev[next.waterA][next.waterB]=current;
				push(next);
			}
		}
		return false;
	}
private:
	int sizeA,sizeB;
	Status *queue;
	int head,tail;
	bool **visited;
	int **step,**operationFrom,**operationTo;
	Status **prev,**next;
};

int main(){
	int sizeA,sizeB,target;
	while(scanf("%d%d%d",&sizeA,&sizeB,&target)!=EOF){
		BFSQueue bfs(sizeA,sizeB);
		bfs.bfs(target);
	}
	return 0;
}