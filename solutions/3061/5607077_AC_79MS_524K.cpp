#include<cstdio>
#include<limits.h>

const int LENGTH_MAX=100000;
int sequence[LENGTH_MAX];
int length,limit;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		scanf("%d%d",&length,&limit);
		for(int i=0;i<length;i++)
			scanf("%d",sequence+i);
		int result=INT_MAX,end=0,sum=0;
		for(int begin=0;begin<length;begin++){
			for(;sum<limit && end<length;end++)
				sum+=sequence[end];
			if(sum<limit)
				break;
			if(result>end-begin)
				result=end-begin;
			sum-=sequence[begin];
		}
		if(result==INT_MAX)
			result=0;
		printf("%d\n",result);
	}
	return 0;
}