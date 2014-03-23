#include<cstdio>

int main(){
	int caseNum;
	scanf("%d",&caseNum);getchar();
	while(caseNum-->0){
		char given[1001];
		gets(given);
		for(int begin=0,count;given[begin]!='\0';begin+=count){
			for(count=1;given[begin+count]==given[begin];count++);
			printf("%d%c",count,given[begin]);
		}
		printf("\n");
	}
	return 0;
}