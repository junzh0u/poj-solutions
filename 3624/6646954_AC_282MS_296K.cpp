#include<cstdio>
#include<algorithm>

const int CHARM_NUM_MAX=3500,BRACELET_SIZE_MAX=13000;

int main(){
	int charmNum,braceletSize;
	scanf("%d%d",&charmNum,&braceletSize);
	int weight[CHARM_NUM_MAX],desirability[CHARM_NUM_MAX];
	for(int charmID=0;charmID<charmNum;charmID++)
		scanf("%d%d",weight+charmID,desirability+charmID);
	int rating[2][BRACELET_SIZE_MAX];
	std::fill(rating[0],rating[0]+BRACELET_SIZE_MAX,0);
	std::fill(rating[1],rating[1]+BRACELET_SIZE_MAX,0);
	int current=0,prev=1;
	for(int charmID=charmNum-1;charmID>=0;charmID--){
		for(int capacity=braceletSize;capacity>=0;capacity--){
			rating[current][capacity]=rating[prev][capacity];
			if(capacity>=weight[charmID]){
				int temp=rating[prev][capacity-weight[charmID]]+desirability[charmID];
				if(rating[current][capacity]<temp)
					rating[current][capacity]=temp;
			}
		}
		std::swap(current,prev);
	}
	printf("%d\n",rating[prev][braceletSize]);
	return 0;
}