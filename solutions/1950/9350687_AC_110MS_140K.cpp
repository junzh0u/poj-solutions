#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<climits>

const int N_MAX=15,PRINT_COUNT=20;
const char PLUS='+',MINUS='-',JOIN='.';
int n,count=0;
char ops[N_MAX];

inline int join(int value,int tail){
	if(tail<10)
		return value*10+tail;
	else
		return value*100+tail;
}

void dfs(int depth,int value,int part,int signal){
	if(depth==n-1){
		if(value+signal*part==0){
			count++;
			if(count<=PRINT_COUNT){
				printf("1");
				for(int i=0;i<n-1;i++)
					printf(" %c %d",ops[i],i+2);
				printf("\n");
			}
		}
		return;
	}
	ops[depth]=PLUS;
	dfs(depth+1,value+signal*part,depth+2,1);
	ops[depth]=MINUS;
	dfs(depth+1,value+signal*part,depth+2,-1);
	ops[depth]=JOIN;
	dfs(depth+1,value,join(part,depth+2),signal);
}

int main(){
	ops[N_MAX-1]='\0';
	scanf("%d",&n);
	dfs(0,0,1,1);
	printf("%d\n",count);
	return 0;
}