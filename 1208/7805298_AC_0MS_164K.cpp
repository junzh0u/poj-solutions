#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>
#include<cstring>

const int BLOCK_NUM_MAX=25,COMMAND_LEN=4,NONE=-1;

int blockNum;
int indexes[BLOCK_NUM_MAX],aboves[BLOCK_NUM_MAX],belows[BLOCK_NUM_MAX];

void print(){
	for(int position=0;position<blockNum;position++){
		printf("%d:",position);
		for(int block=indexes[position];block!=NONE;block=aboves[block])
			printf(" %d",block);
		printf("\n");
	}
}

int topOfIndex(int index){
	while(aboves[index]!=NONE)
		index=aboves[index];
	return index;
}

int topOfPosition(int position){
	return topOfIndex(indexes[position]);
}

void moveOn(int indexFrom,int indexTo){
	if(belows[indexFrom]!=NONE){
		aboves[belows[indexFrom]]=NONE;
		belows[indexFrom]=NONE;
	}
	else{
		for(int position=0;position<blockNum;position++)
			if(indexes[position]==indexFrom)
				indexes[position]=NONE;
	}
	int top=topOfIndex(indexTo);
	aboves[top]=indexFrom;
	belows[indexFrom]=top;
}

void returnToInitial(int index){
	aboves[index]=NONE;
	if(indexes[index]==NONE){
		indexes[index]=index;
		belows[index]=NONE;
	}
	else{
		int top=topOfPosition(index);
		aboves[top]=index;
		belows[index]=top;
	}
}

void returnAboves(int index){
	int toReturns[BLOCK_NUM_MAX],toReturnCount=0;
	for(int above=aboves[index];above!=NONE;above=aboves[above])
		toReturns[toReturnCount++]=above;
	aboves[index]=NONE;
	for(int i=0;i<toReturnCount;i++)
		returnToInitial(toReturns[i]);
}

int main(){
	scanf("%d",&blockNum);
	for(int i=0;i<blockNum;i++){
		indexes[i]=i;
		aboves[i]=belows[i]=NONE;
	}

	while(true){
		char command[2][COMMAND_LEN+1];
		scanf("%s",command[0]);
		if(strcmp(command[0],"quit")==0)
			break;
		int from,to;
		scanf("%d%s%d",&from,command[1],&to);
		if(from==to || topOfIndex(from)==topOfIndex(to))
			continue;
		if(strcmp(command[0],"move")==0)
			returnAboves(from);
		if(strcmp(command[1],"onto")==0)
			returnAboves(to);
		moveOn(from,to);
#ifdef DEBUG
		printf("# After %s %d %s %d:\n",command[0],from,command[1],to);
		print();
#endif
	}
	print();
	return 0;
}