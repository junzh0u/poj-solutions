#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int N_MAX=250;
unsigned count[N_MAX][N_MAX];

int main(){
	for(int min=0;min<N_MAX;min++)
		count[0][min]=1;
	for(int n=0;n<N_MAX;n++)
		count[n][n]=1;
	for(int n=1;n<N_MAX;n++){
		for(int min=n+1;min<N_MAX;min++)
			count[n][min]=0;
		for(int min=n;min>=1;min--){
			if(n<2*min)
				count[n][min]=1;
			else
				count[n][min]=count[n-2*min][min]+count[n][min+1];
		}
	}
	while(true){
		int n;
		scanf("%d",&n);
		if(!n)
			return 0;
		printf("%d %u\n",n,count[n][1]);
	}
}