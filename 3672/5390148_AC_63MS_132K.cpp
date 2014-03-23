#include<cstdio>

int main(){
	const char UPHILL='u',FLAT='f',DOWNHILL='d';
	int timeLimit,pathLength,timeUphill,timeFlat,timeDownhill;
	scanf("%d%d%d%d%d",&timeLimit,&pathLength,&timeUphill,&timeFlat,&timeDownhill);
	int timeUsed=0,farthestDistance=0;
	bool stopped=false;
	for(int i=0;i<pathLength;i++){
		char segment[2];
		scanf("%s",segment);
		if(!stopped){
			int timeNeed=(segment[0]==FLAT)?(2*timeFlat):(timeUphill+timeDownhill);
			if(timeUsed+timeNeed<=timeLimit){
				timeUsed+=timeNeed;
				farthestDistance++;
			}
			else
				stopped=true;
		}
	}
	printf("%d\n",farthestDistance);
	return 0;
}