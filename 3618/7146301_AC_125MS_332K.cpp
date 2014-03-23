#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

bool distanceLess(const int left,const int right){
	return abs(left)<abs(right);
}

int main(){
	int timeLimit,landmarkNum;
	scanf("%d%d",&timeLimit,&landmarkNum);
	int *landmarkPositions=new int[landmarkNum];
	for(int landmarkIndex=0;landmarkIndex<landmarkNum;landmarkIndex++)
		scanf("%d",landmarkPositions+landmarkIndex);
	std::sort(landmarkPositions,landmarkPositions+landmarkNum,distanceLess);
	int currentPosition=0,timeUsed=0,landmarkVisitedNum=0;
	while(landmarkVisitedNum<landmarkNum){
		timeUsed+=abs(landmarkPositions[landmarkVisitedNum]-currentPosition);
		if(timeUsed>timeLimit)
			break;
		currentPosition=landmarkPositions[landmarkVisitedNum];
		landmarkVisitedNum++;
	}
	printf("%d\n",landmarkVisitedNum);
	delete landmarkPositions;
	return 0;
}