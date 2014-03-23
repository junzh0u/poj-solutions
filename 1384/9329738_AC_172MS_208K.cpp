#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

struct Coin{int value,weight;};
const int WEIGHT_MAX=10000,COIN_NUM_MAX=500;
int weightTarget,coinNum;
Coin coins[COIN_NUM_MAX];
int valueMin[WEIGHT_MAX+1];

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int weightEmpty,weightFilled;
		scanf("%d%d",&weightEmpty,&weightFilled);
		weightTarget=weightFilled-weightEmpty;
		scanf("%d",&coinNum);
		for(int i=0;i<coinNum;i++)
			scanf("%d%d",&coins[i].value,&coins[i].weight);
		std::fill(valueMin,valueMin+weightTarget+1,INT_MAX);
		valueMin[0]=0;
		for(int weight=0;weight<=weightTarget;weight++)
			for(int i=0;i<coinNum;i++)
				if(coins[i].weight<=weight && valueMin[weight-coins[i].weight]!=INT_MAX)
					valueMin[weight]=std::min(valueMin[weight],valueMin[weight-coins[i].weight]+coins[i].value);
		if(valueMin[weightTarget]!=INT_MAX)
			printf("The minimum amount of money in the piggy-bank is %d.\n",valueMin[weightTarget]);
		else
			printf("This is impossible.\n");
	}
	return 0;
}