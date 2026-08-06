#include<cstdio>
#include<deque>
#include<algorithm>
using namespace std;

typedef pair<int,bool> PIB;
typedef deque<PIB> DP;

bool PibCmp(PIB pibLeft,PIB pibRight){return pibLeft.first<pibRight.first;}

void main()
{
	int iCaseNum,iJobNum,iPosition,i,iTime;PIB pibJob;DP dpDeque;
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d%d",&iJobNum,&iPosition);dpDeque.clear();
		for(i=0;i<iJobNum;i++)
		{scanf("%d",&pibJob.first);pibJob.second=(i==iPosition);dpDeque.push_back(pibJob);}
		iTime=0;
		while(true)
		{
			if(max_element(dpDeque.begin(),dpDeque.end(),PibCmp)!=dpDeque.begin())
				dpDeque.push_back(*dpDeque.begin());
			else{iTime++;if(dpDeque.begin()->second)break;}
			dpDeque.pop_front();
		}
		printf("%d\n",iTime);
	}
}