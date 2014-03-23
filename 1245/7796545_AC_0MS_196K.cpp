#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<functional>
using namespace std;

const int TEAM_NUM_MAX=20,NAME_LEN_MAX=10,PROBLEM_NUM=7;

class Team{
public:
	char name[NAME_LEN_MAX+1];
	int times[PROBLEM_NUM];
	int solveNum,timeSum,timeMean;
	int rank;
public:
	void read(){
		solveNum=timeSum=timeMean=0;
		double temp=0.0;
		scanf("%s",name);
		for(int i=0;i<PROBLEM_NUM;i++){
			scanf("%d",times+i);
			if(times[i]){
				solveNum++;
				timeSum+=times[i];
				temp+=log((double)times[i]);
			}
		}
		if(solveNum){
			temp/=solveNum;
			temp=exp(temp);
			timeMean=(int)(temp+0.5);
		}
	}
	void print(){
		"01 Plutonians 7 1992 261 123 234 345 456 167 278 389";
		printf("%.2d %-10s%2d%5d%4d",rank,name,solveNum,timeSum,timeMean);
		for(int i=0;i<PROBLEM_NUM;i++)
			printf("%4d",times[i]);
		printf("\n");
	}
	bool operator>(const Team& another) const{
		if(solveNum!=another.solveNum)
			return solveNum>another.solveNum;
		if(timeSum!=another.timeSum)
			return timeSum<another.timeSum;
		if(timeMean!=another.timeMean)
			return timeMean<another.timeMean;
		return strcmp(name,another.name)<0;
	}
	bool operator==(const Team& another) const{
		return solveNum==another.solveNum && timeSum==another.timeSum && timeMean==another.timeMean;
	}
};

int teamNum;
Team teams[TEAM_NUM_MAX];

int main(){
	int contestID=0;
	while(true){
		scanf("%d",&teamNum);
		if(teamNum==0)
			return 0;
		for(int i=0;i<teamNum;i++)
			teams[i].read();
		sort(teams,teams+teamNum,greater<Team>());
		teams[0].rank=1;
		for(int i=1;i<teamNum;i++){
			if(teams[i]==teams[i-1])
				teams[i].rank=teams[i-1].rank;
			else
				teams[i].rank=i+1;
		}
		printf("CONTEST %d\n",++contestID);
		for(int i=0;i<teamNum;i++)
			teams[i].print();
	}
}