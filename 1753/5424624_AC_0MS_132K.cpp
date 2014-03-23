#include<cstdio>
#include<algorithm>
using namespace std;

//BLACK-1 WHITE-0
const int SIZE=4,LOW_4_MASK=15;

int OperateCount(int operate){
	int count=0;
	while(operate!=0){
		if(operate&1!=0)
			count++;
		operate>>=1;
	}
	return count;
}

void Flip(char state[SIZE],int row,int operate){
	if(row-1>=0)
		state[row-1]^=operate;
	state[row]^=operate^(operate<<1)^(operate>>1);
	state[row]&=LOW_4_MASK;
	if(row+1<SIZE)
		state[row+1]^=operate;
}

int OperateMin(const char stateBegin[SIZE]){
	int operateMin=INT_MAX,state;
	for(int operateFirst=0;operateFirst<=LOW_4_MASK;operateFirst++){
		char state[SIZE];
		copy(stateBegin,stateBegin+SIZE,state);
		Flip(state,0,operateFirst);
		int operateCount=OperateCount(operateFirst);
		for(int row=1;row<SIZE;row++){
			int operate=state[row-1];
			Flip(state,row,operate);
			operateCount+=OperateCount(operate);
		}
		if(state[SIZE-1]==0 && operateMin>operateCount)
			operateMin=operateCount;
	}
	return operateMin;
}

int main(){
	char stateBegin[SIZE];
	for(int row=0;row<SIZE;row++){
		char line[SIZE+1];
		scanf("%s",line);
		stateBegin[row]=0;
		for(int j=0;j<SIZE;j++)
			if(line[j]=='b')
				stateBegin[row]|=1<<j;
		stateBegin[row]&=LOW_4_MASK;
	}
	int operateMin=OperateMin(stateBegin);
	for(int row=0;row<SIZE;row++)
		stateBegin[row]^=LOW_4_MASK;
	operateMin=min(operateMin,OperateMin(stateBegin));
	if(operateMin==INT_MAX)
		printf("Impossible\n");
	else
		printf("%d\n",operateMin);
	return 0;
}