#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int ARRAY_SIZE_MAX=1000000,WINDOW_SIZE_MAX=ARRAY_SIZE_MAX;
int arraySize,windowSize,array[ARRAY_SIZE_MAX];

int begin,end,queue[ARRAY_SIZE_MAX];
void pushMin(int index){
	while(end>begin && array[queue[end-1]]>=array[index])
		end--;
	queue[end++]=index;
}
void pushMax(int index){
	while(end>begin && array[queue[end-1]]<=array[index])
		end--;
	queue[end++]=index;
}
int pop(int lowerBound){
	while(queue[begin]<lowerBound)
		begin++;
	return queue[begin];
}

int main(){
	scanf("%d%d",&arraySize,&windowSize);
	for(int i=0;i<arraySize;i++)
		scanf("%d",array+i);
	begin=end=0;
	for(int i=0;i<windowSize-1;i++)
		pushMin(i);
	for(int i=0;i<=arraySize-windowSize;i++){
		pushMin(i+windowSize-1);
		printf("%d ",array[pop(i)]);
	}
	printf("\n");
	begin=end=0;
	for(int i=0;i<windowSize-1;i++)
		pushMax(i);
	for(int i=0;i<=arraySize-windowSize;i++){
		pushMax(i+windowSize-1);
		printf("%d ",array[pop(i)]);
	}
	printf("\n");
	return 0;
}