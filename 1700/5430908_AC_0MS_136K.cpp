#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int peopleNum;
		scanf("%d",&peopleNum);
		int *timeNeed=new int[peopleNum];
		for(int i=0;i<peopleNum;i++)
			scanf("%d",timeNeed+i);
		int timeTotal;
		if(peopleNum==1)
			timeTotal=timeNeed[0];
		else{
			if(peopleNum%2==0)
				timeTotal=timeNeed[1];
			else
				timeTotal=timeNeed[0]+timeNeed[1]+timeNeed[2];
			sort(timeNeed,timeNeed+peopleNum);
			for(int i=peopleNum-1;i>=3;i-=2)
				timeTotal+=min(timeNeed[1]+timeNeed[0]+timeNeed[i]+timeNeed[1],timeNeed[i]+timeNeed[0]+timeNeed[i-1]+timeNeed[0]);
			delete[] timeNeed;
		}
		printf("%d\n",timeTotal);
	}
	return 0;
}