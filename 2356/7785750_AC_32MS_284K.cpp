#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int N_MAX=10000,UNDEF=-1;
int n,numbers[N_MAX],sums[N_MAX+1],sumToEnd[N_MAX+1];

int print(int begin,int end){
	printf("%d\n",end-begin);
	for(int i=begin;i<end;i++)
		printf("%d\n",numbers[i]);
	return 0;
}

int main(){
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		scanf("%d",numbers+i);
	sums[0]=0;
	std::fill(sumToEnd,sumToEnd+n,UNDEF);
	for(int end=1;end<=n;end++){
		sums[end]=(sums[end-1]+numbers[end-1])%n;
		if(sums[end]==0)
			return print(0,end);
		if(sumToEnd[sums[end]]!=EOF)
			return print(sumToEnd[sums[end]],end);
		sumToEnd[sums[end]]=end;
	}
	printf("0\n");
	return 0;
}