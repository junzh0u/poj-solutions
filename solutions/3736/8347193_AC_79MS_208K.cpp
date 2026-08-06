#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;

const double DEST_X=100.0,DEST_Y=0.0,TIME_ARRIVE_SNIPER=100.0;

class Enemy{
public:
	double x,y,stun,range,damage,timeArrive;
public:
	Enemy(){
		x=y=stun=range=damage=timeArrive=0.0;
	}
	void input(){
		cin>>x>>y>>stun>>range>>damage;
		timeArrive=sqrt((x-DEST_X)*(x-DEST_X)+(y-DEST_Y)*(y-DEST_Y))-range;
		if(timeArrive<0.0)
			timeArrive=0.0;
	}
	bool operator<(const Enemy& a){
		return timeArrive<a.timeArrive;
	}
};

int main(){
	int enemyNum;
	while(cin>>enemyNum){
		vector<Enemy> enemies(enemyNum);
		for(int i=0;i<enemyNum;i++)
			enemies[i].input();
		double sniperHp;
		cin>>sniperHp;
		sort(enemies.begin(),enemies.end());
		double time=TIME_ARRIVE_SNIPER;
		for(vector<Enemy>::iterator iter=enemies.begin();iter!=enemies.end();iter++){
			if(iter->timeArrive>time)
				break;
			time+=iter->stun;
			sniperHp-=iter->damage;
			if(sniperHp<=0.0)
				break;
		}
		cout<<(sniperHp>0.0?"Safe!":"Danger!")<<endl;
	}
	return 0;
}