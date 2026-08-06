#include<iostream>
#include<algorithm>
#include<functional>
using namespace std;

const int NODE_NUM_MAX=200;

class StoneGame
{
public:
	StoneGame(){};
	bool Input();
	void Output();
private:
	int StonePicked(int);
	//static const int NODE_NUM_MAX=200;
	int nodeNum,childNum[NODE_NUM_MAX],childLabel[NODE_NUM_MAX][NODE_NUM_MAX];
};

bool StoneGame::Input()
{
	cin>>nodeNum;
	for(int node=0;node<nodeNum;node++)
	{
		int label;
		cin>>label>>childNum[node];
		for(int i=0;i<childNum[node];i++)
		{
			cin>>childLabel[node][i];
			childLabel[node][i]--;
		}
	}
	return true;
}

void StoneGame::Output()
{
	cout<<StonePicked(0)<<endl;
}

int StoneGame::StonePicked(int node)
{
	if(childNum[node]==0)
		return 1;
	int childStone[NODE_NUM_MAX];
	for(int i=0;i<childNum[node];i++)
		childStone[i]=StonePicked(childLabel[node][i]);
	sort(childStone,childStone+childNum[node],greater<int>());
	int stone=0,stoneRemain=0;
	for(i=0;i<childNum[node];i++)
	//for(int i=0;i<childNum[node];i++)
	{
		if(childStone[i]>stoneRemain)
		{
			stone+=childStone[i]-stoneRemain;
			stoneRemain=0;
		}
		else
			stoneRemain-=childStone[i];
		stoneRemain+=childStone[i]-1;
	}
	return stone;
}

int main()
{
	int treeNum;
	cin>>treeNum;
	while((treeNum--)>0)
	{
		StoneGame testCase;
		testCase.Input();
		testCase.Output();
	}
	return 0;
}