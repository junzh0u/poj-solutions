#include<iostream>
#include<algorithm>
using namespace std;

const int VILLAGE_NUM_MAX=100,INF=100000;

int main(){
	int villageNum,distance[VILLAGE_NUM_MAX][VILLAGE_NUM_MAX];
	cin>>villageNum;
	for(int from=0;from<villageNum;from++)
		for(int to=0;to<villageNum;to++)
			cin>>distance[from][to];
	int roadExist;
	cin>>roadExist;
	while(roadExist-->0){
		int from,to;
		cin>>from>>to;
		distance[from-1][to-1]=distance[to-1][from-1]=0;
	}
	int distanceMin[VILLAGE_NUM_MAX];
	for(int i=0;i<villageNum;i++)
		distanceMin[i]=INF;
	distanceMin[0]=0;
	bool connected[VILLAGE_NUM_MAX]={false};
	int lengthTotal=0;
	for(int step=0;step<villageNum;step++){
		int choose=INF;
		for(int village=0;village<villageNum;village++)
			if(!connected[village] && (choose==INF || distanceMin[choose]>distanceMin[village]))
				choose=village;
		connected[choose]=true;
		lengthTotal+=distanceMin[choose];
		for(int next=0;next<villageNum;next++)
			if(distanceMin[next]>distance[choose][next])
				distanceMin[next]=distance[choose][next];
	}
	cout<<lengthTotal<<endl;
	return 0;
}