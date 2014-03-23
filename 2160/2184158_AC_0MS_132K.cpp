#include<iostream>
using namespace std;

struct Pallet{int iWidth,iLength;} pSide[6];

int PalCmp(const void* pLeft,const void* pRight)
{
	if(((Pallet*)pLeft)->iWidth!=((Pallet*)pRight)->iWidth)
		return ((Pallet*)pLeft)->iWidth-((Pallet*)pRight)->iWidth;
	return ((Pallet*)pLeft)->iLength-((Pallet*)pRight)->iLength;
}

bool Judge()
{
	for(int i=0;i<3;i++)if(PalCmp(&pSide[2*i],&pSide[2*i+1])!=0)return false;
	return pSide[0].iWidth==pSide[2].iWidth && pSide[0].iLength==pSide[4].iWidth && pSide[2].iLength==pSide[4].iLength;
}

void main()
{
	for(int i=0;i<6;i++)
	{
		cin>>pSide[i].iWidth>>pSide[i].iLength;
		if(pSide[i].iWidth>pSide[i].iLength)swap(pSide[i].iWidth,pSide[i].iLength);
	}
	qsort(pSide,6,sizeof(Pallet),PalCmp);
	cout<<(Judge()?"POSSIBLE":"IMPOSSIBLE")<<endl;
}