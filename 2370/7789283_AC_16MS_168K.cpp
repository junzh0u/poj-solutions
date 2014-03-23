#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>

int main(){
	int groupNum;
	scanf("%d",&groupNum);
	int *groupSizes=new int[groupNum];
	for(int groupIndex=0;groupIndex<groupNum;groupIndex++)
		scanf("%d",groupSizes+groupIndex);
	std::sort(groupSizes,groupSizes+groupNum);
	int supporterNeeded=0;
	for(int groupIndex=0;groupIndex<groupNum/2+1;groupIndex++)
		supporterNeeded+=groupSizes[groupIndex]/2+1;
	printf("%d\n",supporterNeeded);
	delete[] groupSizes;
	return 0;
}