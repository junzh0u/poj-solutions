#include<cstdio>
#include<algorithm>

int slayDragon(int *headDiameters,int headNum,int *knightHeights,int knightNum){
	if(headNum>knightNum)
		return -1;
	if(headNum==0)
		return knightHeights[0];
	int knightIndex=0,coinNum=0;
	for(int headIndex=0;headIndex<headNum;headIndex++){
		while(knightIndex<knightNum && headDiameters[headIndex]>knightHeights[knightIndex])
			knightIndex++;
		if(knightIndex>=knightNum)
			return -1;
		coinNum+=knightHeights[knightIndex++];
	}
	return coinNum;
}

int main(){
	int headNum,knightNum;
	while(true){
		scanf("%d%d",&headNum,&knightNum);
		if(headNum==0 && knightNum==0)
			return 0;
		int *headDiameters=new int[headNum];
		for(int headIndex=0;headIndex<headNum;headIndex++)
			scanf("%d",headDiameters+headIndex);
		int *knightHeights=new int[knightNum];
		for(int knightIndex=0;knightIndex<knightNum;knightIndex++)
			scanf("%d",knightHeights+knightIndex);
		std::sort(headDiameters,headDiameters+headNum);
		std::sort(knightHeights,knightHeights+knightNum);
		int coinNum=slayDragon(headDiameters,headNum,knightHeights,knightNum);
		if(coinNum==-1)
			printf("Loowater is doomed!\n");
		else
			printf("%d\n",coinNum);
		delete[] knightHeights;
		delete[] headDiameters;
	}
}