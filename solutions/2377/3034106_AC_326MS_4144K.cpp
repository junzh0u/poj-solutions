#include<iostream>
#include<algorithm>
using namespace std;

const int VECTOR_NUM_MAX=1000;
int vectorNum,edgeNum;
bool visited[VECTOR_NUM_MAX]={false};
int edgeMax[VECTOR_NUM_MAX];
int edge[VECTOR_NUM_MAX][VECTOR_NUM_MAX]={0};

int main(){
	cin>>vectorNum>>edgeNum;
	while(edgeNum--){
		int from,to,cost;
		cin>>from>>to>>cost;
		from--;
		to--;
		if(edge[from][to]<cost)
			edge[from][to]=edge[to][from]=cost;
	}
	visited[0]=true;
	copy(edge[0],edge[0]+vectorNum,edgeMax);
	int edgeSum=0,step;
	for(step=1;step<vectorNum;step++){
		int next=0;
		for(int i=0;i<vectorNum;i++)
			if(edgeMax[next]<edgeMax[i])
				next=i;
		if(visited[next])
			break;
		edgeSum+=edgeMax[next];
		visited[next]=true;
		for(i=0;i<vectorNum;i++){
			if(visited[i])
				edgeMax[i]=0;
			else if(edgeMax[i]<edge[next][i])
				edgeMax[i]=edge[next][i];
		}
	}
	if(step<vectorNum)
		cout<<-1<<endl;
	else
		cout<<edgeSum<<endl;
	return 0;
}