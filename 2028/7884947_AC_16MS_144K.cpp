#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int DATE_MAX=100;

int main(){
	while(true){
		int memberNum,quorum;
		scanf("%d%d",&memberNum,&quorum);
		if(!memberNum && !quorum)
			return 0;
		int convenientCount[DATE_MAX+1];
		std::fill(convenientCount,convenientCount+DATE_MAX+1,0);
		while(memberNum--){
			int dateNum;
			scanf("%d",&dateNum);
			while(dateNum--){
				int date;
				scanf("%d",&date);
				convenientCount[date]++;
			}
		}
		int *date=std::max_element(convenientCount,convenientCount+DATE_MAX+1);
		if(*date<quorum)
			printf("0\n");
		else
			printf("%d\n",date-convenientCount);
	}
}