#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int N_MAX=12;

int main(){
	int factorial[N_MAX]={1};
	for(int i=1;i<N_MAX;i++)
		factorial[i]=factorial[i-1]*i;
	int caseNum;
	scanf("%d",&caseNum);
	for(int caseId=1;caseId<=caseNum;caseId++){
		int n,k,array[N_MAX];
		scanf("%d%d",&n,&k);
		for(int i=0;i<k;i++)
			scanf("%d",array+i);
		int pos=1;
		for(int i=0;i<k;i++){
			pos+=(array[i]-1)*factorial[n-i-1]/factorial[n-k];
			for(int j=i+1;j<k;j++)
				if(array[j]>array[i])
					array[j]--;
		}
		printf("Variace cislo %d ma poradove cislo %d.\n",caseId,pos);
	}
	return 0;
}