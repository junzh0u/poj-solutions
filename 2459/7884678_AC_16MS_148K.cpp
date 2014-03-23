#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int DAY_MAX=2000;

int main(){
	int cowNum,recieve,remain,now;
	scanf("%d%d%d%d",&cowNum,&recieve,&remain,&now);
	int cowNumAtDay[DAY_MAX];
	std::fill(cowNumAtDay,cowNumAtDay+DAY_MAX,0);
	while(cowNum--){
		int arrive,leave;
		scanf("%d%d",&arrive,&leave);
		for(int day=arrive-1;day<=leave-1;day++)
			cowNumAtDay[day]++;
	}
	while(remain<recieve){
		remain+=cowNumAtDay[now-1];
		now--;
	}
	printf("%d\n",now+1);
	return 0;
}