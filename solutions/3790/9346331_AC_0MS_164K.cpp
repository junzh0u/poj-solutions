#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

int main(){
	const int N_MAX=1000;
	int answers[N_MAX+1]={1,1};
	for(int n=2;n<=N_MAX;n++){
		if(n%2)
			answers[n]=answers[n-2]+answers[n/2];
		else
			answers[n]=answers[n-1]+answers[n/2];
	}
	int caseNum;
	scanf("%d",&caseNum);
	for(int id=1;id<=caseNum;id++){
		int n;
		scanf("%d",&n);
		printf("%d %d\n",id,answers[n]);
	}
	return 0;
}