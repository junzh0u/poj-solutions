#include<iostream>
using namespace std;

const int NODE_NUM_MAX=18;
int treeNum[NODE_NUM_MAX+1],treeNumSum[NODE_NUM_MAX+1];

void Print(int* array,int size)
{
	for(int i=0;i<size;i++)
		cout<<i<<"\t"<<array[i]<<endl;
}

void Initialize()
{
	treeNum[0]=treeNumSum[0]=1;
	for(int nodeNum=1;nodeNum<=NODE_NUM_MAX;nodeNum++)
	{
		treeNum[nodeNum]=0;
		for(int leftNodeNum=0;leftNodeNum<nodeNum;leftNodeNum++)
			treeNum[nodeNum]+=treeNum[leftNodeNum]*treeNum[nodeNum-1-leftNodeNum];
		treeNumSum[nodeNum]=treeNumSum[nodeNum-1]+treeNum[nodeNum];
	}
	//Print(treeNum,NODE_NUM_MAX+1);
	//Print(treeNumSum,NODE_NUM_MAX+1);
}

void PrintTree(int orderNum,int nodeNum)
{
	if(nodeNum==0)
		return;
	int leftNodeNum=0,orderBefore=0;
	for(leftNodeNum=0;leftNodeNum<nodeNum;leftNodeNum++)
	{
		if(orderBefore+treeNum[leftNodeNum]*treeNum[nodeNum-1-leftNodeNum]>=orderNum)
			break;
		orderBefore+=treeNum[leftNodeNum]*treeNum[nodeNum-1-leftNodeNum];
	}
	int leftOrderNum=(orderNum-orderBefore-1)/treeNum[nodeNum-1-leftNodeNum]+1;
	int rightOrderNum=(orderNum-orderBefore-1)%treeNum[nodeNum-1-leftNodeNum]+1;
	if(leftNodeNum!=0)
	{
		cout<<"(";
		PrintTree(leftOrderNum,leftNodeNum);
		cout<<")";
	}
	cout<<"X";
	if(nodeNum-1-leftNodeNum!=0)
	{
		cout<<"(";
		PrintTree(rightOrderNum,nodeNum-1-leftNodeNum);
		cout<<")";
	}
}

void main()
{
	Initialize();
	int orderNum;
	while(cin>>orderNum)
	{
		if(orderNum==0)
			break;
		orderNum++;
		int nodeNum=1;
		while(treeNumSum[nodeNum]<orderNum)
			nodeNum++;
		PrintTree(orderNum-treeNumSum[nodeNum-1],nodeNum);
		cout<<endl;
	}
	//system("pause");
}