#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

int main(){
	for(int caseId=1;;caseId++){
		int n0;
		scanf("%d",&n0);
		if(!n0)
			return 0;
		printf("%d. ",caseId);
		int n1=3*n0;
		printf((n1%2)?"odd ":"even ");
		int n2=(n1+1)/2;
		int n3=3*n2;
		int n4=n3/9;
		printf("%d\n",n4);
	}
}