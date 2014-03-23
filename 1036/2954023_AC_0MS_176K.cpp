#include<iostream>
#include<algorithm>
using namespace std;

const int GANGSTER_NUM_MAX=100,INF=-1;

struct Gangster{
	int time,prosperity,stoutness;
}gangsters[GANGSTER_NUM_MAX];

int gangsterNum,stateNum,timeMax,memory[GANGSTER_NUM_MAX];

bool TimeEarlier(Gangster left,Gangster right){
	return left.time<right.time;
}

int ProsperitySum(int lastEnter){
	if(memory[lastEnter]!=INF)
		return memory[lastEnter];
	if(gangsters[lastEnter].stoutness>gangsters[lastEnter].time)
		return 0;
	int sumMax=gangsters[lastEnter].prosperity;
	for(int before=lastEnter-1;before>=0;before--)
		if(abs(gangsters[lastEnter].time-gangsters[before].time)>=
			abs(gangsters[lastEnter].stoutness-gangsters[before].stoutness))
		{
			int sum=ProsperitySum(before);
			if(sumMax<sum+gangsters[lastEnter].prosperity)
				sumMax=sum+gangsters[lastEnter].prosperity;
		}
	return memory[lastEnter]=sumMax;
}

int main(){
	cin>>gangsterNum>>stateNum>>timeMax;
	for(int i=0;i<gangsterNum;i++)
		cin>>gangsters[i].time;
	for(i=0;i<gangsterNum;i++)
		cin>>gangsters[i].prosperity;
	for(i=0;i<gangsterNum;i++)
		cin>>gangsters[i].stoutness;
	sort(gangsters,gangsters+gangsterNum,TimeEarlier);
	memset(memory,INF,sizeof(memory));
	int prosperitySumMax=0;
	for(i=0;i<gangsterNum;i++){
		int sum=ProsperitySum(i);
		if(prosperitySumMax<sum)
			prosperitySumMax=sum;
	}
	cout<<prosperitySumMax<<endl;
	return 0;
}