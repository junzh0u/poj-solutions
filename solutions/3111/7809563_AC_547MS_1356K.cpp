#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<cmath>
#include<algorithm>

const int JEWEL_NUM_MAX=100000;

int jewelNum,keep;
double averageValue;

struct Jewel{
	int index,value,weight;
	bool operator<(const Jewel &another){
		//return value-weight*averageValue>another.value-another.weight*averageValue;
		return value-another.value>(weight-another.weight)*averageValue;
	}
};
Jewel jewels[JEWEL_NUM_MAX];

double getAverageValue(){
	double valueSum=0.0,weightSum=0.0;
	for(int i=0;i<keep;i++){
		valueSum+=jewels[i].value;
		weightSum+=jewels[i].weight;
	}
	return valueSum/weightSum;
}

int main(){
	scanf("%d%d",&jewelNum,&keep);
	for(int i=0;i<jewelNum;i++){
		jewels[i].index=i;
		scanf("%d%d",&jewels[i].value,&jewels[i].weight);
	}
	if(jewelNum!=keep){
		averageValue=getAverageValue();
		while(true){
			std::nth_element(jewels,jewels+keep,jewels+jewelNum);
			double newAverageValue=getAverageValue();
			if(fabs(averageValue-newAverageValue)<=1e-8)
				break;
			averageValue=newAverageValue;
		}
	}
	/*
	printf("%d",jewels[0].index+1);
	for(int i=1;i<keep;i++)
		printf(" %d",jewels[i].index+1);
	*/
	for(int i=0;i<keep;i++)
		printf("%d ",jewels[i].index+1);
	printf("\n");
	return 0;
}