#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int ARRAY_SIZE_MAX=30;
int size;
int array[ARRAY_SIZE_MAX];

const int FLIP_NUM_MAX=ARRAY_SIZE_MAX*2;
int flipNum;
int flips[FLIP_NUM_MAX];

void output(){
	printf("%d",flipNum);
	for(int i=0;i<flipNum;i++)
		printf(" %d",flips[i]);
	printf("\n");
}

void flip(int top){
	flips[flipNum++]=top;
	std::reverse(array,array+top);
#ifdef _DEBUG
	printf("#Flip %d\n",top);
	for(int i=0;i<size;i++)
		printf("%d ",array[i]);
	printf("\n");
#endif
}

void sort(){
	flipNum=0;
	for(int value=size;value>=2;value--){
		int position=std::find(array,array+size,value)-array;
		if(position==value-1)
			continue;
		if(position)
			flip(position+1);
		flip(value);
	}
}

bool input(){
	scanf("%d",&size);
	if(!size)
		return false;
	for(int i=0;i<size;i++)
		scanf("%d",array+i);
	return true;
}

int main(){
	while(input()){
		sort();
		output();
	}
	return 0;
}