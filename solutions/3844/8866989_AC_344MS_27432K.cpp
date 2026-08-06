#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<ctime>
#include<algorithm>

int main(){
#ifdef _DEBUG
	clock_t begin=clock();
#endif
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int divisor,length;
		scanf("%d%d",&divisor,&length);
		int *sum=new int[length+1];
		sum[0]=0;
		for(int i=0;i<length;i++){
			int element;
			scanf("%d",&element);
			sum[i+1]=(sum[i]+element)%divisor;
		}
		int *sumCount=new int[divisor];
		std::fill(sumCount,sumCount+divisor,0);
		sumCount[0]=1;
		int count=0;
		for(int i=1;i<=length;i++){
			count+=sumCount[sum[i]];
			sumCount[sum[i]]++;
		}
		delete[] sum;
		printf("%d\n",count);
	}
#ifdef _DEBUG
	clock_t end=clock();
	printf("%d ms\n",end-begin);
#endif
	return 0;
}