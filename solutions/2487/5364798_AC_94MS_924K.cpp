#include<cstdio>
#include<algorithm>
#include<functional>
using namespace std;

int main(){
	int scenarioNum;
	scanf("%d",&scenarioNum);
	for(int ID=1;ID<=scenarioNum;ID++){
		int need,friendNum;
		scanf("%d%d",&need,&friendNum);
		int *friends=new int[friendNum];
		for(int i=0;i<friendNum;i++)
			scanf("%d",friends+i);
		sort(friends,friends+friendNum,greater<int>());
		int sum=0,count=0;
		while(count<friendNum && sum<need)
			sum+=friends[count++];
		printf("Scenario #%d:\n",ID);
		if(sum<need)
			printf("impossible\n\n");
		else
			printf("%d\n\n",count);
	}
	return 0;
}