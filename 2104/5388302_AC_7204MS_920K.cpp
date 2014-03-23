#include<cstdio>
#include<algorithm>
using namespace std;

struct Data{
	int num,index;
};

bool DataLess(const Data& left,const Data& right){
	return left.num<right.num;
}

int main(){
	int arraySize,questionNum;
	scanf("%d%d",&arraySize,&questionNum);
	//Data *array=new Data[arraySize];
	Data array[100000];
	for(int i=0;i<arraySize;i++){
		array[i].index=i;
		scanf("%d",&array[i].num);
	}
	sort(array,array+arraySize,DataLess);
	while(questionNum-->0){
		int begin,end,ordinal;
		scanf("%d%d%d",&begin,&end,&ordinal);
		for(int i=0;i<arraySize;i++){
			if(array[i].index>=begin-1 && array[i].index<=end-1)
				ordinal--;
			if(ordinal==0){
				printf("%d\n",array[i].num);
				break;
			}
		}
	}
	return 0;
}