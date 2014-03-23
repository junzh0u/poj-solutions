#include<cstdio>
#include<algorithm>

const int PROBLEM_NUM_MAX=30;

int caseNum;
int contestLast,problemNum,problemCost[PROBLEM_NUM_MAX];

std::pair<int,int> solve(){
	int solved=0,score=0,spent=0;
	for(int i=0;i<problemNum;i++){
		spent+=problemCost[i];
		if(spent>contestLast)
			break;
		solved++;
		score-=spent;
	}
	return std::pair<int,int>(solved,score);
}

int main(){
	scanf("%d",&caseNum);
	for(int caseId=1;caseId<=caseNum;caseId++){
		scanf("%d%d",&contestLast,&problemNum);
		for(int i=0;i<problemNum;i++)
			scanf("%d",problemCost+i);
		std::sort(problemCost,problemCost+problemNum);
		std::pair<int,int> steve=solve();
		printf("Scenario #%d:\n",caseId);
		printf("Steve wins with %d solved problems and a score of %d.\n\n",steve.first,-steve.second);
	}
	return 0;
}