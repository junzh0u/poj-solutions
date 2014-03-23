#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<memory>
#ifdef _DEBUG
#include<ctime>
#include<cassert>
#endif

struct Status{
	int expGreater,expLess;
};
const int QUEUE_SIZE=500000,GREATER_LIMIT=30000,LESS_LIMIT=100;
Status queue[QUEUE_SIZE];
int begin,end;
int endMax=0,beginMax=0;

int step[GREATER_LIMIT+1][LESS_LIMIT+1];

inline void transform(int method,const Status &from,Status &to){
	switch(method){
	case 0:
		to.expGreater=from.expGreater-from.expLess;
		to.expLess=from.expGreater;
		break;
	case 1:
		to.expGreater=from.expGreater-from.expLess;
		to.expLess=from.expLess;
		break;
	case 2:
		to.expGreater=from.expGreater+from.expLess;
		to.expLess=from.expGreater;
		break;
	case 3:
		to.expGreater=from.expGreater+from.expLess;
		to.expLess=from.expLess;
		break;
	case 4:
		to.expGreater=from.expGreater+from.expGreater;
		to.expLess=from.expGreater;
		break;
	case 5:
		to.expGreater=from.expGreater+from.expGreater;
		to.expLess=from.expLess;
		break;
	case 6:
		to.expGreater=from.expLess+from.expLess;
		to.expLess=from.expGreater;
		break;
	case 7:
		to.expGreater=from.expLess+from.expLess;
		to.expLess=from.expLess;
		break;
#ifdef _DEBUG
	default:
		assert(true);
		break;
#endif
	}
	if(to.expGreater<to.expLess)
		std::swap(to.expGreater,to.expLess);
}

int solve(int target){
	if(target==0 || target==1)
		return 0;
	memset(step,-1,sizeof(int)*(GREATER_LIMIT+1)*(LESS_LIMIT+1));
	begin=0;
	queue[begin].expGreater=1;
	queue[begin].expLess=0;
	step[1][0]=0;
	end=1;
	while(begin!=end){
		for(int method=0;method<8;method++){
			transform(method,queue[begin],queue[end]);
			if(queue[end].expGreater==target || queue[end].expLess==target)
				return step[queue[begin].expGreater][queue[begin].expLess]+1;
			if(queue[end].expGreater>GREATER_LIMIT || queue[end].expLess>LESS_LIMIT)
				continue;
			if(step[queue[end].expGreater][queue[end].expLess]==-1){
				step[queue[end].expGreater][queue[end].expLess]=step[queue[begin].expGreater][queue[begin].expLess]+1;
				end=(end+1)%QUEUE_SIZE;
			}
		}
		begin=(begin+1)%QUEUE_SIZE;
	}
	return -1;
}


int main(){
#ifdef _DEBUG
	const int TEST_BEGIN=18673,TEST_NUM=100;
	for(int target=TEST_BEGIN;target<TEST_BEGIN+TEST_NUM;target++){
		clock_t timeBegin=clock();
		int step=solve(target);
		clock_t timeEnd=clock();
		if(endMax<end){
			endMax=end;
			beginMax=begin;
		}
		printf("%6d ms:\tReach %d at %d steps.\n",timeEnd-timeBegin,target,step);
	}
	printf("Max used queue size: %d (-%d)\n",endMax,beginMax);
#endif
	int target;
	scanf("%d",&target);
	printf("%d\n",solve(target));
	return 0;
}