#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>
#include<queue>

const int STONE_NUM_MAX=1200,UNDEF=-1;

struct Follower{
	int num;
	int indexes[STONE_NUM_MAX];
	void input(){
		scanf("%d",&num);
		for(int i=0;i<num;i++){
			scanf("%d",indexes+i);
			indexes[i]--;
		}
	}
};

int stoneNum;
Follower followers[STONE_NUM_MAX];
int timeMax,stoneToPush;

int timeFall[STONE_NUM_MAX],fallenNum,lastTime;
int queue[STONE_NUM_MAX*2],queueBegin,queueEnd;

void fall(int stone,int time){
	queue[queueEnd++]=stone;
	timeFall[stone]=time;
	fallenNum++;
	if(lastTime==UNDEF || lastTime<time)
		lastTime=time;
}

int timeDestruct(int push){
	std::fill(timeFall,timeFall+stoneNum,UNDEF);
	fallenNum=0;
	lastTime=UNDEF;
	queueBegin=queueEnd=0;
	
	fall(push,0);
	while(queueEnd!=queueBegin){
		int current=queue[queueBegin++];
		for(int followerIndex=0;followerIndex<followers[current].num;followerIndex++){
			int next=followers[current].indexes[followerIndex];
			if(timeFall[next]!=UNDEF)
				continue;
			fall(next,timeFall[current]+1);
			if(fallenNum==stoneNum)
				return lastTime;
		}
	}
	
	if(fallenNum==stoneNum)
		return lastTime;

	return UNDEF;
}

void process(){
	timeMax=UNDEF;
	stoneToPush=UNDEF;
	for(int push=0;push<stoneNum;push++){
		int time=timeDestruct(push);
		if(time==UNDEF)
			continue;
		if(timeMax==UNDEF || timeMax<=time){
			timeMax=time;
			stoneToPush=push;
		}
	}
}

void input(){
	scanf("%d",&stoneNum);
	for(int i=0;i<stoneNum;i++)
		followers[i].input();
}

void output(){
	if(timeMax==UNDEF)
		printf("impossible\n");
	else
		printf("%d\n%d\n",timeMax,stoneToPush+1);
}

int main(){
	input();
	process();
	output();
	return 0;
}