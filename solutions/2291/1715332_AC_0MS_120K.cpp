#include<iostream>
#include<algorithm>

using namespace std;

void main()
{
	int iCaseNum,iRopeNum,iRope[1000],i,j,iWeiMax;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iRopeNum;
		for(j=0;j<iRopeNum;j++)
			cin>>iRope[j];
		sort(iRope,iRope+iRopeNum);
		iWeiMax=0;
		for(j=0;j<iRopeNum;j++)
			if(iRope[j]*(iRopeNum-j)>iWeiMax)
				iWeiMax=iRope[j]*(iRopeNum-j);
		cout<<iWeiMax<<endl;
	}
}
