#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int CHAIN_LEN_MAX=12;
int chain[CHAIN_LEN_MAX],chainShortestLen,chainShortest[CHAIN_LEN_MAX];

void dfs(const int chainLen,int *chain,const int n){
	if(chainLen>=CHAIN_LEN_MAX)
		return;
	if(chainLen>=chainShortestLen)
		return;
	for(int i=chainLen-1;i>=0;i--){
		chain[chainLen]=chain[i]+chain[chainLen-1];
		if(chain[chainLen]==n){
			chainShortestLen=chainLen+1;
			std::copy(chain,chain+chainShortestLen,chainShortest);
		}
		else if(chain[chainLen]<n)
			dfs(chainLen+1,chain,n);
	}
}

int main(){
	int n;
	while(true){
		scanf("%d",&n);
		if(!n)
			return 0;
		if(n==1){
			printf("1\n");
			continue;
		}
		int chainLen=1,chain[CHAIN_LEN_MAX]={1};
		chainShortestLen=INT_MAX;
		dfs(chainLen,chain,n);
		for(int i=0;i<chainShortestLen;i++)
			printf("%d ",chainShortest[i]);
		printf("\n");
	}
}