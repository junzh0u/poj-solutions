#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;

const int SIZE=100,INF=INT_MAX/2;
int difMax;
int itemNum;
vector<int> stands;
set<int> standSet;
struct Exchange{
	int from,to,cost;
};
vector<Exchange> edges;

bool StandCmp(int left,int right){
	return stands[left]<stands[right];
}

set<int> CreateSet(int standMin){
	set<int> result;
	for(unsigned i=0;i<stands.size();i++)
		if(stands[i]>=standMin && stands[i]<=standMin+difMax)
			result.insert(i);
	return result;
}

int Bellman(set<int> deal){
	vector<int> cost(itemNum+1,INF);
	cost[0]=0;
	while(true){
		bool anyChange=false;
		for(unsigned i=0;i<edges.size();i++)
			if(deal.find(edges[i].from)!=deal.end() && deal.find(edges[i].to)!=deal.end() &&
				cost[edges[i].to]>cost[edges[i].from]+edges[i].cost){
					anyChange=true;
					cost[edges[i].to]=cost[edges[i].from]+edges[i].cost;
				}
		if(!anyChange)
			break;
	}
	return cost[1];
}

int main(){
	cin>>difMax>>itemNum;
	stands.resize(itemNum+1);
	for(int i=1;i<=itemNum;i++){
		Exchange change={0,i,0};
		int changeNum;
		cin>>change.cost>>stands[i]>>changeNum;
		standSet.insert(stands[i]);
		edges.push_back(change);
		while(changeNum--){
			cin>>change.from>>change.cost;
			edges.push_back(change);
		}
	}
	stands[0]=stands[1];
	int minCost=INF;
	for(set<int>::iterator iter=standSet.begin();iter!=standSet.end();iter++){
		if(*iter>stands[1])
			break;
		if(*iter+difMax>=stands[1]){
			set<int> deal=CreateSet(*iter);
			int cost=Bellman(deal);
			if(minCost>cost)
				minCost=cost;
		}
		if(*iter+difMax>=*standSet.rbegin())
			break;
	}
	cout<<minCost<<endl;
	return 0;
}