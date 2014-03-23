#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>

const int COMPUTER_NUM_MAX=200;
const int TRUCK_NUM_MAX=COMPUTER_NUM_MAX;

long long shemeNum[COMPUTER_NUM_MAX+1][TRUCK_NUM_MAX+1];

int computerNum,truckNum;

int main(){
	for(computerNum=0;computerNum<=COMPUTER_NUM_MAX;computerNum++)
		for(truckNum=0;truckNum<=TRUCK_NUM_MAX;truckNum++){
			if(computerNum<truckNum)
				shemeNum[computerNum][truckNum]=0;
			else if(truckNum==0)
				shemeNum[computerNum][truckNum]=0;
			else if(truckNum==1)
				shemeNum[computerNum][truckNum]=1;
			else
				shemeNum[computerNum][truckNum]=shemeNum[computerNum-1][truckNum-1]+shemeNum[computerNum-truckNum][truckNum];
		}
	while(true){
		scanf("%d%d",&computerNum,&truckNum);
		if(computerNum==0 && truckNum==0)
			return 0;
		printf("%lld\n",shemeNum[computerNum][truckNum]);
	}
}