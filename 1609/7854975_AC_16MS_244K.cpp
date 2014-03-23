#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int L_MAX=100,M_MAX=100;

int blockNum;
int blockCount[L_MAX+1][M_MAX+1],tileCount[L_MAX+1][M_MAX+1];

int main(){
	while(true){
		scanf("%d",&blockNum);
		if(!blockNum){
			printf("*\n");
			return 0;
		}
		for(int l=0;l<=L_MAX;l++)
			for(int m=0;m<=M_MAX;m++)
				blockCount[l][m]=0;
		while(blockNum--){
			int l,m;
			scanf("%d%d",&l,&m);
			blockCount[l][m]++;
		}
		for(int l=0;l<=L_MAX;l++)
			for(int m=0;m<=M_MAX;m++)
				tileCount[l][m]=0;
		for(int l=1;l<=L_MAX;l++)
			for(int m=1;m<=M_MAX;m++)
				tileCount[l][m]=blockCount[l][m]+std::max(tileCount[l][m-1],tileCount[l-1][m]);
		printf("%d\n",tileCount[L_MAX][M_MAX]);
	}
}