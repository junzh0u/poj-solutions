#include<iostream>
#include<string>
using namespace std;
const int MAX_LENGTH=26,NON_PARENT=-1;

class MAryTree
{
public:
	MAryTree(){};
	bool Read();
	void Print();
private:
	void Solve();
	void Analyze(string,string);
	void CountSon();
	int Choose(int,int);
	//static const int MAX_LENGTH=26,NON_PARENT=-1;
	int aryNum,length,parent[MAX_LENGTH],sonNum[MAX_LENGTH],possibleTreeNum;
	string preOrder,postOrder;
};

bool MAryTree::Read()
{
	cin>>aryNum;
	if(aryNum==0)
		return false;
	cin>>preOrder>>postOrder;
	return true;
}

void MAryTree::Print()
{
	Solve();
	cout<<possibleTreeNum<<endl;
}

void MAryTree::Solve()
{
	length=preOrder.length();
	parent[preOrder[0]-'a']=NON_PARENT;
	Analyze(preOrder,postOrder);
	CountSon();
	possibleTreeNum=1;
	for(int node=0;node<length;node++)
		possibleTreeNum*=Choose(aryNum,sonNum[node]);
}

void MAryTree::Analyze(string preOrder,string postOrder)
{
	while(preOrder.length()>1)
	{
		char root=preOrder[0],son=preOrder[1];
		parent[son-'a']=root-'a';
		int length=postOrder.find(son)+1;
		Analyze(preOrder.substr(1,length),postOrder.substr(0,length));
		preOrder.erase(1,length);
		postOrder.erase(0,length);
	}
}

void MAryTree::CountSon()
{
	memset(sonNum,0,sizeof(sonNum));
	for(int node=0;node<length;node++)
		if(parent[node]!=NON_PARENT)
			sonNum[parent[node]]++;
}

int MAryTree::Choose(int total,int part)
{
	int result=1;
	int i;
	for(i=0;i<part;i++)
		result*=total-i;
	for(i=1;i<=part;i++)
		result/=i;
	/*for(int i=0;i<part;i++)
		result*=total-i;
	for(int i=1;i<=part;i++)
		result/=i;*/
	return result;
}

void main()
{
	MAryTree instance;
	while(instance.Read())
		instance.Print();
}