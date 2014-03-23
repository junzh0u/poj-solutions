#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

int main(){
	unsigned cityNum;
	scanf("%u",&cityNum);
	unsigned prev,min=INT_MAX,minCity;
	scanf("%u",&prev);
	for(unsigned city=2;city<cityNum-1;city++){
		unsigned current;
		scanf("%u",&current);
		if(min>current-prev){
			min=current-prev;
			minCity=city;
		}
		prev=current;
	}
	scanf("%d",&prev);
	if(cityNum<=3)
		printf("0\n");
	else
		printf("%u\n%u 1 %u %u\n",prev+min,minCity+1,cityNum,minCity);
	return 0;
}