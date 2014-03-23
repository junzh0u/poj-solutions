#include<cstdio>
#include<algorithm>

const int COW_NUM_MAX=100,SMART_MIN=-1000,SMART_MAX=1000;
const int SMART_SUM_MIN=COW_NUM_MAX*SMART_MIN,SMART_SUM_MAX=COW_NUM_MAX*SMART_MAX;
const int SMART_SUM_RANGE=SMART_SUM_MAX-SMART_SUM_MIN+1;

int cowNum,funSumMax[2][SMART_SUM_RANGE],smarts[COW_NUM_MAX],funs[COW_NUM_MAX];

int currentLine=0;
bool legal(int smart){return smart>=SMART_SUM_MIN && smart<=SMART_SUM_MAX;}
int &current(int smart){return funSumMax[currentLine][smart-SMART_SUM_MIN];}
int &prev(int smart){return funSumMax[1-currentLine][smart-SMART_SUM_MIN];}
void swap(){currentLine=1-currentLine;}

int main(){
	for(int i=0;i<2;i++)
		std::fill(funSumMax[i],funSumMax[i]+SMART_SUM_RANGE,INT_MIN);
	current(0)=0;
	scanf("%d",&cowNum);
	int smartSumMax=0,smartSumMin=0;
	for(int i=0;i<cowNum;i++){
		scanf("%d%d",smarts+i,funs+i);
		if(smarts[i]<0)
			smartSumMin+=smarts[i];
		if(smarts[i]>0)
			smartSumMax+=smarts[i];
	}
	for(int i=0;i<cowNum;i++){
		swap();
		for(int smartSum=smartSumMin;smartSum<=smartSumMax;smartSum++){
			current(smartSum)=prev(smartSum);
			if(legal(smartSum-smarts[i]) && prev(smartSum-smarts[i])!=INT_MIN && current(smartSum)<prev(smartSum-smarts[i])+funs[i])
				current(smartSum)=prev(smartSum-smarts[i])+funs[i];
		}
	}
	int sumMax=0;
	for(int smartSum=0;smartSum<=SMART_SUM_MAX;smartSum++)
		if(current(smartSum)>=0 && sumMax<smartSum+current(smartSum))
			sumMax=smartSum+current(smartSum);
	printf("%d\n",sumMax);
	return 0;
}