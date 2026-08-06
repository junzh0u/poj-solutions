#include<cstdio>
#include<queue>
#include<deque>
#include<algorithm>
using namespace std;

int FordFulkerson(int** flow,int pointNum){
	queue<int> points;
	int *prev=new int[pointNum];
	bool *visited=new bool[pointNum];
	fill(prev,prev+pointNum,-1);
	fill(visited,visited+pointNum,false);
	points.push(0);
	visited[0]=true;
	while(!points.empty()){
		int current=points.front();
		points.pop();
		if(current==pointNum-1){
			deque<int> path;
			while(current!=-1){
				path.push_front(current);
				current=prev[current];
			}
			int minFlow=INT_MAX;
			for(int i=0;i<path.size()-1;i++)
				if(minFlow>flow[path[i]][path[i+1]])
					minFlow=flow[path[i]][path[i+1]];
			for(int i=0;i<path.size()-1;i++){
				flow[path[i]][path[i+1]]-=minFlow;
				flow[path[i+1]][path[i]]+=minFlow;
			}
			return minFlow;
		}
		for(int next=0;next<pointNum;next++)
			if(flow[current][next]>0 && !visited[next]){
				points.push(next);
				prev[next]=current;
				visited[next]=true;
			}
	}
	return 0;
}

int main(){
	int ditchNum,pointNum;
	while(scanf("%d%d",&ditchNum,&pointNum)!=EOF){
		int **flow=new int*[pointNum];
		for(int i=0;i<pointNum;i++){
			flow[i]=new int[pointNum];
			fill(flow[i],flow[i]+pointNum,0);
		}
		while(ditchNum-->0){
			int from,to,rate;
			scanf("%d%d%d",&from,&to,&rate);
			flow[from-1][to-1]+=rate;
		}
		int maxRate=0;
		while(true){
			int rate=FordFulkerson(flow,pointNum);
			if(rate==0)
				break;
			maxRate+=rate;
		}
		printf("%d\n",maxRate);
		for(int i=0;i<pointNum;i++)
			delete[] flow[i];
		delete[] flow;
	}
	return 0;
}