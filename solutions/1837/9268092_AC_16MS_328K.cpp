#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int HOOK_NUM_MAX=20,WEIGHT_NUM_MAX=20;
const int HOOK_MAX=15,WEIGHT_MAX=25;
const int MOMENT_MAX=WEIGHT_NUM_MAX*WEIGHT_NUM_MAX*HOOK_MAX;

int hookNum,weightNum;
int hooks[HOOK_NUM_MAX],weights[WEIGHT_NUM_MAX];
int possibilities[HOOK_NUM_MAX][MOMENT_MAX+1]={0};

int main(){
	scanf("%d%d",&hookNum,&weightNum);
	for(int i=0;i<hookNum;i++)
		scanf("%d",hooks+i);
	for(int i=0;i<weightNum;i++)
		scanf("%d",weights+i);
	for(int i=0;i<hookNum;i++)
		possibilities[0][MOMENT_MAX/2+weights[0]*hooks[i]]=1;
	int weightSum=weights[0];
	for(int w=1;w<weightNum;w++){
		int momentMin=std::max(hooks[0]*weightSum,-MOMENT_MAX/2)+MOMENT_MAX/2,
			momentMax=std::min(hooks[hookNum-1]*weightSum,MOMENT_MAX/2)+MOMENT_MAX/2;
		for(int h=0;h<hookNum;h++)
			for(int moment=momentMin;moment<=momentMax;moment++)
				possibilities[w][moment+weights[w]*hooks[h]]+=possibilities[w-1][moment];
		weightSum+=weights[w];
	}
	printf("%d\n",possibilities[weightNum-1][MOMENT_MAX/2]);
	return 0;
}