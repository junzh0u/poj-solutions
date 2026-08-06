#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

const int PEOPLE_NUM_MAX=8;

bool canProcedure(int coconutNum,int peopleNum){
	for(int wake=0;wake<peopleNum;wake++){
		if(coconutNum%peopleNum!=1)
			return false;
		coconutNum-=coconutNum/peopleNum+1;
	}
	if(coconutNum%peopleNum)
		return false;
	return true;
}

int getPeopleNumMax(int coconutNum){
	for(int peopleNum=PEOPLE_NUM_MAX;peopleNum>=2;peopleNum--)
		if(canProcedure(coconutNum,peopleNum))
			return peopleNum;
	return -1;
}

int main(){
	while(true){
		int coconutNum;
		scanf("%d",&coconutNum);
		if(coconutNum==-1)
			return 0;
		int peopleNum=getPeopleNumMax(coconutNum);
		if(peopleNum==-1)
			printf("%d coconuts, no solution\n",coconutNum);
		else
			printf("%d coconuts, %d people and 1 monkey\n",coconutNum,peopleNum);
	}
}