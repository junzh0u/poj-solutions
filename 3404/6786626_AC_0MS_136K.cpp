#include<cstdio>
#include<algorithm>

int main(){
	int travelerNum;
	scanf("%d",&travelerNum);
	int *timeCosts=new int[travelerNum];
	for(int index=0;index<travelerNum;index++)
		scanf("%d",timeCosts+index);
	std::sort(timeCosts,timeCosts+travelerNum);
	int timeSum=0;
	while(travelerNum>=4){
		int timePlanA=timeCosts[0]+2*timeCosts[1]+timeCosts[travelerNum-1];
		int timePlanB=2*timeCosts[0]+timeCosts[travelerNum-2]+timeCosts[travelerNum-1];
		timeSum+=std::min(timePlanA,timePlanB);
		travelerNum-=2;
	}
	if(travelerNum==3)
		timeSum+=timeCosts[0]+timeCosts[1]+timeCosts[2];
	else if(travelerNum==2)
		timeSum+=std::max(timeCosts[0],timeCosts[1]);
	else if(travelerNum==1)
		timeSum+=timeCosts[0];
	printf("%d\n",timeSum);
	delete[] timeCosts;
	return 0;
}