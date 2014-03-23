#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>

const int CAKE_NUM_MAX=4500,BASE=1000000007;

int main(){
	int plateNum,cakeNum;
	scanf("%d%d",&plateNum,&cakeNum);
	int wayNum[2][CAKE_NUM_MAX+1];
	for(int cake=0;cake<=cakeNum;cake++)
		wayNum[1][cake]=1;
	for(int plate=2;plate<=plateNum;plate++){
		for(int cake=0;cake<=cakeNum;cake++){
			wayNum[plate%2][cake]=wayNum[(plate-1)%2][cake];
			if(plate<=cake){
				wayNum[plate%2][cake]+=wayNum[plate%2][cake-plate];
				wayNum[plate%2][cake]%=BASE;
			}
		}
	}
	printf("%d",wayNum[plateNum%2][cakeNum]);
	return 0;
}