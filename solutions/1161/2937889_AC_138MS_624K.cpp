#include<iostream>
using namespace std;

const int REGION_NUM_MAX=200,TOWN_NUM_MAX=250,CLUB_MEMBER_NUM_MAX=30,INF=INT_MAX/2-1;
int regionNum,townNum,clubMemberNum;
int clubMember[CLUB_MEMBER_NUM_MAX],wallDivide[TOWN_NUM_MAX][TOWN_NUM_MAX],regionDistance[REGION_NUM_MAX][REGION_NUM_MAX];
bool townAroundRegion[TOWN_NUM_MAX][REGION_NUM_MAX];

bool Initialize(){
	for(int i=0;i<clubMemberNum;i++)
		clubMember[i]=INF;
	for(i=0;i<townNum;i++){
		for(int j=0;j<townNum;j++)
			wallDivide[i][j]=INF;
		for(j=0;j<regionNum;j++)
			townAroundRegion[i][j]=false;
	}
	for(i=0;i<regionNum;i++)
		for(int j=0;j<regionNum;j++)
			regionDistance[i][j]=(i==j)?0:INF;
	return true;
}

int main(){
	cin>>regionNum>>townNum>>clubMemberNum;
	Initialize();
	for(int member=0;member<clubMemberNum;member++){
		cin>>clubMember[member];
		clubMember[member]--;
	}
	for(int region=0;region<regionNum;region++){
		int townAroundNum,prev,first;
		cin>>townAroundNum>>first;
		first--;
		townAroundRegion[first][region]=true;
		prev=first;
		for(int town=1;town<townAroundNum;town++){
			int current;
			cin>>current;
			current--;
			townAroundRegion[current][region]=true;
			if(wallDivide[prev][current]!=INF)
				regionDistance[region][wallDivide[prev][current]]=regionDistance[wallDivide[prev][current]][region]=1;
			else
				wallDivide[prev][current]=wallDivide[current][prev]=region;
			prev=current;
		}
		if(wallDivide[prev][first]!=INF)
			regionDistance[region][wallDivide[prev][first]]=regionDistance[wallDivide[prev][first]][region]=1;
		else
			wallDivide[prev][first]=wallDivide[first][prev]=region;
	}
	/*for(int i=0;i<regionNum;i++){
		for(int j=0;j<regionNum;j++){
			if(regionDistance[i][j]==INF)
				cout<<'*';
			else
				cout<<regionDistance[i][j];
			cout<<"\t";
		}
		cout<<endl;
	}*/
	for(int mid=0;mid<regionNum;mid++)
		for(int from=0;from<regionNum;from++)
			for(int to=0;to<regionNum;to++)
				if(regionDistance[from][to]>regionDistance[from][mid]+regionDistance[mid][to])
					regionDistance[from][to]=regionDistance[from][mid]+regionDistance[mid][to];
	/*for(int i=0;i<regionNum;i++){
		for(int j=0;j<regionNum;j++){
			if(regionDistance[i][j]==INF)
				cout<<'*';
			else
				cout<<regionDistance[i][j];
			cout<<"\t";
		}
		//cout<<endl;
	}*/
	int distanceSumMin=INF;
	for(region=0;region<regionNum;region++){
		int distanceSum=0;
		for(int member=0;member<clubMemberNum;member++){
			int distance=INF;
			for(int around=0;around<regionNum;around++)
				if(townAroundRegion[clubMember[member]][around] && distance>regionDistance[region][around])
					distance=regionDistance[region][around];
			distanceSum+=distance;
		}
		if(distanceSumMin>distanceSum)
			distanceSumMin=distanceSum;
	}
	cout<<distanceSumMin<<endl;
	return 0;
}