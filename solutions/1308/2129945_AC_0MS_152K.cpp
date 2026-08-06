#include<iostream>
#include<vector>
using namespace std;
#define MAX 20
typedef vector <int> VI;

VI viLeft,viRight,viNode[MAX];

bool Input()
{
	int iLeft,iRight;
	viLeft.clear();viRight.clear();
	while(cin>>iLeft>>iRight)
	{
		if(iLeft==-1)return false;
		if(iLeft==0)return true;
		viLeft.push_back(iLeft);viRight.push_back(iRight);
	}
	return true;
}

void Visit(int iCode,bool* bVisited)
{
	if(bVisited[iCode])return;
	bVisited[iCode]=true;
	for(int i=0;i<viNode[iCode].size();i++)
		Visit(viNode[iCode][i],bVisited);
}

bool Solve()
{
	bool bExist[MAX]={false},bVisited[MAX]={false};
	int i,iNodeCount=0;
	if(viLeft.empty())return true;
	for(i=0;i<MAX;i++)viNode[i].clear();
	for(i=0;i<viLeft.size();i++)
	{
		bExist[viLeft[i]]=bExist[viRight[i]]=true;
		viNode[viLeft[i]].push_back(viRight[i]);
		viNode[viRight[i]].push_back(viLeft[i]);
	}
	for(i=0;i<MAX;i++)
		if(bExist[i])
		{
			iNodeCount++;
			Visit(i,bVisited);
		}
	if(iNodeCount!=viLeft.size()+1)return false;
	for(i=0;i<MAX;i++)
		if(bExist[i]!=bVisited[i])
			return false;
	return true;
}

void main()
{
	int iCaseCount=0;
	while(Input())
	{
		cout<<"Case "<<++iCaseCount<<" is "<<(Solve()?"":"not ")<<"a tree."<<endl;
	}
}