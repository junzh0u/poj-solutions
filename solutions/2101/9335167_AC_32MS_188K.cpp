#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cmath>

int main(){
	int etwNum,ntsNum;
	scanf("%d%d",&etwNum,&ntsNum);
	double etwLen=0.0,ntsLen=0.0;
	for(int i=1;i<etwNum;i++){
		int d;
		scanf("%d",&d);
		etwLen+=d;
	}
	for(int i=1;i<ntsNum;i++){
		int d;
		scanf("%d",&d);
		ntsLen+=d;
	}
	printf("%d\n",(int)ceil(sqrt(etwLen*etwLen+ntsLen*ntsLen)));
	return 0;
}