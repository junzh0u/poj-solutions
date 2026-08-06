#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<cmath>

const int POWER_MAX=1024,STEP_MAX=13;

int target;

struct Status{
	int power;
	int step;
	int computed[STEP_MAX+1];
};

int stepMin[POWER_MAX];

void dfs(Status current){
	stepMin[current.power-1]=current.step;
	Status next=current;
	next.step++;
	if(next.step>=STEP_MAX)
		return;
	for(int i=current.step;i>=0;i--){
		for(int j=i;j>=0;j--){
			next.power=current.computed[i]+current.computed[j];
			if(next.power>POWER_MAX)
				continue;
			if(stepMin[next.power-1]<next.step)
				continue;
			next.computed[next.step]=next.power;
			dfs(next);
		}
	}
	for(int i=current.step;i>=1;i--){
		for(int j=i-1;j>=0;j--){
			next.power=current.computed[i]-current.computed[j];
			if(stepMin[next.power-1]<next.step)
				continue;
			next.computed[next.step]=next.power;
			dfs(next);
		}
	}
}

int main(){
	for(int power=1;power<=POWER_MAX;power++){
		int len=(int)ceil(log((double)power)/log(2.0));
		int digitOneCount=0;
		int temp=power;
		while(temp){
			if(temp&1)
				digitOneCount++;
			temp>>=1;
		}
		stepMin[power-1]=len+digitOneCount;
		if(stepMin[power-1]>STEP_MAX)
			stepMin[power-1]=STEP_MAX;
	}
	Status start={1,0,{1}};
	dfs(start);
	while(true){
		scanf("%d",&target);
		if(target==0)
			return 0;
		printf("%d\n",stepMin[target-1]);
	}
}