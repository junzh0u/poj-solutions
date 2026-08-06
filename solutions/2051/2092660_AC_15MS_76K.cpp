#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>

using namespace std;

struct Query
{
	int iID,iPeriod,iNext;
};

bool Compare(Query pLeft,Query pRight)
{
	if(pLeft.iNext>pRight.iNext)
		return true;
	else if(pLeft.iNext<pRight.iNext)
		return false;
	else if(pLeft.iID>pRight.iID)
		return true;
	else
		return false;
}

void main()
{
	char cTemp[20],cCommand[10];
	int iCount;
	Query qTemp;
	vector<Query> vqStream;
	while(gets(cTemp))
	{
		if(cTemp[0]=='#')
			break;
		sscanf(cTemp,"%s %d %d",cCommand,&(qTemp.iID),&(qTemp.iPeriod));
		qTemp.iNext=qTemp.iPeriod;
		vqStream.push_back(qTemp);
	}
	make_heap(vqStream.begin(),vqStream.end(),Compare);
	scanf("%d",&iCount);
	while(iCount--)
	{
		printf("%d\n",vqStream[0].iID);
		pop_heap(vqStream.begin(),vqStream.end(),Compare);
		(vqStream.end()-1)->iNext+=(vqStream.end()-1)->iPeriod;
		push_heap(vqStream.begin(),vqStream.end(),Compare);
	}
}