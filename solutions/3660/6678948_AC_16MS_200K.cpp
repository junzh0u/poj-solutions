#include<cstdio>

const int DISTANCE_MAX=10000;

int main(){
	int cowNum,roundNum;
	scanf("%d%d",&cowNum,&roundNum);
	int **distance=new int*[cowNum];
	for(int from=0;from<cowNum;from++){
		distance[from]=new int[cowNum];
		for(int to=0;to<cowNum;to++)
			distance[from][to]=DISTANCE_MAX;
	}
	while(roundNum--){
		int from,to;
		scanf("%d%d",&from,&to);
		distance[from-1][to-1]=1;
	}
	for(int mid=0;mid<cowNum;mid++)
		for(int from=0;from<cowNum;from++)
			for(int to=0;to<cowNum;to++)
				if(distance[from][to]>distance[from][mid]+distance[mid][to])
					distance[from][to]=distance[from][mid]+distance[mid][to];
	int determinedCount=0;
	for(int index=0;index<cowNum;index++){
		int fromCount=0;
		for(int from=0;from<cowNum;from++)
			if(distance[from][index]<DISTANCE_MAX)
				fromCount++;
		int toCount=0;
		for(int to=0;to<cowNum;to++)
			if(distance[index][to]<DISTANCE_MAX)
				toCount++;
		if(fromCount+toCount==cowNum-1)
			determinedCount++;
	}
	printf("%d\n",determinedCount);
	return 0;
}