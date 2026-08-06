#include<cstdio>
#include<cmath>

int main(){
	int pageTotal,cowNum;
	scanf("%d%d",&pageTotal,&cowNum);
	while(cowNum-->0){
		int pagePerMinute,consecutiveTime,restTime;
		scanf("%d%d%d",&pagePerMinute,&consecutiveTime,&restTime);
		int timeNeed=(pageTotal+pagePerMinute-1)/pagePerMinute;
		int restCount=(timeNeed+consecutiveTime-1)/consecutiveTime-1;
		int timeTotal=timeNeed+restCount*restTime;
		printf("%d\n",timeTotal);
	}
	return 0;
}