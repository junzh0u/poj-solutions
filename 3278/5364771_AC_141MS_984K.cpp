#include<cstdio>
#include<queue>
using namespace std;

struct Status{
	int position,time;
};

int main(){
	const int POSITION_MAX=100001;
	int john,cow;
	scanf("%d%d",&john,&cow);
	queue<Status> BFS;
	Status begin={john,0};
	BFS.push(begin);
	bool visited[POSITION_MAX];
	fill(visited,visited+POSITION_MAX,false);
	visited[john]=true;
	while(true){
		Status current=BFS.front();
		BFS.pop();
		if(current.position==cow){
			printf("%d\n",current.time);
			break;
		}
		Status next;
		next.time=current.time+1;
		if(current.position-1>=0 && !visited[current.position-1]){
			next.position=current.position-1;
			BFS.push(next);
			visited[current.position-1]=true;
		}
		if(current.position+1<POSITION_MAX && !visited[current.position+1]){
			next.position=current.position+1;
			BFS.push(next);
			visited[current.position+1]=true;
		}
		if(current.position*2<POSITION_MAX && !visited[current.position*2]){
			next.position=current.position*2;
			BFS.push(next);
			visited[current.position*2]=true;
		}
	}
	return 0;
}