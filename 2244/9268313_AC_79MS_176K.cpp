#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

struct City{
	int id,prev,next;
};

const int CITY_NUM_MAX=150;
City cities[CITY_NUM_MAX];

bool simulate(int cityNum,int step){
	for(int i=0;i<cityNum;i++){
		cities[i].id=i+1;
		cities[i].next=(i+1)%cityNum;
		cities[i].prev=(i+cityNum-1)%cityNum;
	}
	int pos=0,cityRemain=cityNum;
	while(cityRemain>1){
		if(cities[pos].id==2)
			return false;
		cities[cities[pos].prev].next=cities[pos].next;
		cities[cities[pos].next].prev=cities[pos].prev;
		cityRemain--;
		for(int i=0;i<(step-1)%cityRemain+1;i++)
			pos=cities[pos].next;
	}
	return true;
}

int main(){
	int cityNum;
	while(true){
		scanf("%d",&cityNum);
		if(!cityNum)
			return 0;
		for(int step=2;;step++){
			if(simulate(cityNum,step)){
				printf("%d\n",step);
				break;
			}
		}
	}
}