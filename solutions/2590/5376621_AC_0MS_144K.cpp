#include<cstdio>
#include<cmath>
#include<algorithm>
using namespace std;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int from,to;
		scanf("%d%d",&from,&to);
		long distance=to-from;
		if(distance==0){
			printf("0\n");
			continue;
		}
		long k=sqrt((double)distance);
		if(k*k>=distance)
			printf("%d\n",2*k-1);
		else if(k*(k+1)>=distance)
			printf("%d\n",2*k);
		else
			printf("%d\n",2*k+1);
	}
	return 0;
}