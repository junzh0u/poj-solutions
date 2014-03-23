#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int COW_NUM_MAX=100,PASTURE_NUM_MAX=1000;
int cowNum,pastureNum,pathNum;
int position[COW_NUM_MAX];
bool connected[PASTURE_NUM_MAX][PASTURE_NUM_MAX]={false};
int visitorCount[PASTURE_NUM_MAX]={0};
bool visited[PASTURE_NUM_MAX];

void visit(int pastureID){
	visitorCount[pastureID]++;
	visited[pastureID]=true;
	for(int next=0;next<pastureNum;next++)
		if(connected[pastureID][next] && !visited[next])
			visit(next);
}

int main(){
	scanf("%d%d%d",&cowNum,&pastureNum,&pathNum);
	for(int index=0;index<cowNum;index++){
		scanf("%d",position+index);
		position[index]--;
	}
	while(pathNum--){
		int from,to;
		scanf("%d%d",&from,&to);
		connected[from-1][to-1]=true;
	}
	for(int index=0;index<cowNum;index++){
		std::fill(visited,visited+pastureNum,false);
		visit(position[index]);
	}
	int result=0;
	for(int index=0;index<pastureNum;index++)
		if(visitorCount[index]==cowNum)
			result++;
	printf("%d\n",result);
	return 0;
}