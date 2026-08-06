#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

const int LOCATION_NUM_MAX=30;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int locationNum;
		scanf("%d",&locationNum);
		int distance[LOCATION_NUM_MAX][LOCATION_NUM_MAX];
		for(int from=0;from<locationNum;from++){
			for(int to=0;to<=from;to++)
				distance[from][to]=INT_MAX;
			for(int to=from+1;to<locationNum;to++)
				scanf("%d",distance[from]+to);
		}
		int timeCost[LOCATION_NUM_MAX][LOCATION_NUM_MAX][LOCATION_NUM_MAX];
		for(int firstCar=0;firstCar<locationNum;firstCar++)
			for(int secondCar=0;secondCar<locationNum;secondCar++)
				for(int thirdCar=0;thirdCar<locationNum;thirdCar++)
					timeCost[firstCar][secondCar][thirdCar]=INT_MAX;
		timeCost[0][0][0]=0;
		for(int firstCar=0;firstCar<locationNum;firstCar++){
			for(int secondCar=firstCar;secondCar<locationNum;secondCar++){
				for(int thirdCar=secondCar;thirdCar<locationNum;thirdCar++){
					if(thirdCar==locationNum-1)
						continue;
					if(timeCost[firstCar][secondCar][thirdCar]==INT_MAX)
						continue;
					if(timeCost[secondCar][thirdCar][thirdCar+1]>timeCost[firstCar][secondCar][thirdCar]+distance[firstCar][thirdCar+1])
						timeCost[secondCar][thirdCar][thirdCar+1]=timeCost[firstCar][secondCar][thirdCar]+distance[firstCar][thirdCar+1];
					if(timeCost[firstCar][thirdCar][thirdCar+1]>timeCost[firstCar][secondCar][thirdCar]+distance[secondCar][thirdCar+1])
						timeCost[firstCar][thirdCar][thirdCar+1]=timeCost[firstCar][secondCar][thirdCar]+distance[secondCar][thirdCar+1];
					if(timeCost[firstCar][secondCar][thirdCar+1]>timeCost[firstCar][secondCar][thirdCar]+distance[thirdCar][thirdCar+1])
						timeCost[firstCar][secondCar][thirdCar+1]=timeCost[firstCar][secondCar][thirdCar]+distance[thirdCar][thirdCar+1];
				}
			}
		}
		int timeCostMin=INT_MAX;
		for(int firstCar=0;firstCar<locationNum;firstCar++)
			for(int secondCar=firstCar;secondCar<locationNum;secondCar++)
				if(timeCostMin>timeCost[firstCar][secondCar][locationNum-1])
					timeCostMin=timeCost[firstCar][secondCar][locationNum-1];
		printf("%d\n",timeCostMin);		
	}
	return 0;
}