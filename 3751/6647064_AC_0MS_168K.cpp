#include<cstdio>

int main(){
	int dateNum;
	scanf("%d",&dateNum);
	while(dateNum--){
		int year,month,day,hour,minute,second;
		scanf("%d/%d/%d-%d:%d:%d",&year,&month,&day,&hour,&minute,&second);
		printf("%02d/%02d/%04d-%02d:%02d:%02d%s\n",month,day,year,(hour+11)%12+1,minute,second,(hour<12)?"am":"pm");
	}
	return 0;
}