#include<iostream.h>
#include<string.h>

struct Node
{
	int iChildNum,iStack,iBegin;
	Node* pnChild[9];
} nRoot;
char cLine[10001];

void Read(Node* pnRoot,int& iPos)
{
	int i,j,iTemp;
	if(cLine[iPos]=='a')
	{
		pnRoot->iChildNum=pnRoot->iBegin=0;
		pnRoot->iStack=1;
	}
	else
	{
		pnRoot->iChildNum=cLine[iPos]-'0';
		for(i=pnRoot->iChildNum-1;i>=0;i--)
		{
			pnRoot->pnChild[i]=new Node;
			Read(pnRoot->pnChild[i],--iPos);
		}
		pnRoot->iStack=100000;pnRoot->iBegin=0;
		for(i=0;i<pnRoot->iChildNum;i++)
		{
			iTemp=0;
			for(j=0;j<pnRoot->iChildNum;j++)
				if(pnRoot->pnChild[(i+j)%pnRoot->iChildNum]->iStack+j>iTemp)
					iTemp=pnRoot->pnChild[(i+j)%pnRoot->iChildNum]->iStack+j;
			if(iTemp<pnRoot->iStack)
			{
				pnRoot->iStack=iTemp;
				pnRoot->iBegin=i;
			}
		}
	}
}

void Print(Node* nRoot)
{
	if(nRoot->iChildNum==0)
		cout<<"a";
	else
	{
		for(int i=0;i<nRoot->iChildNum;i++)
			Print(nRoot->pnChild[(nRoot->iBegin+i)%nRoot->iChildNum]);
		cout<<nRoot->iChildNum;
	}
}

void main()
{
	int iPos;
	cin>>cLine;iPos=strlen(cLine)-1;
	Read(&nRoot,iPos);
	cout<<nRoot.iStack<<endl;
	Print(&nRoot);
	cout<<endl;
}