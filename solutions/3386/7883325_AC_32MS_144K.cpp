#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int main(){
	int outer[2],inner[2],plate;
	scanf("%d%d%d%d%d",outer,inner,outer+1,inner+1,&plate);
	if(	outer[0]+outer[1]<=plate ||
		(inner[0]>=outer[1] && outer[0]<=plate) ||
		(inner[1]>=outer[0] && outer[1]<=plate))
		printf("Yes\n");
	else
		printf("No\n");
	return 0;
}