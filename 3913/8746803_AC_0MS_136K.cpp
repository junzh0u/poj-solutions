#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int main(){
	printf("Gnomes:\n");
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		if((a<b && b<c)||(a>b && b>c))
			printf("Ordered\n");
		else
			printf("Unordered\n");
	}
	return 0;
}