#include<iostream>
#include<algorithm>

using namespace std;

int iStickNum,iStick[64],iOriginLen,iOriginNum;
bool bStick[64];

int GREATER(int,int);
bool CreateStick(int,int,int);

void main()
{
	int iSumLength,i;
	bool bTemp;
	while(1)
	{
		cin>>iStickNum;
		if(!iStickNum)
			break;
		iSumLength=0;
		for(i=0;i<iStickNum;i++)
		{
			cin>>iStick[i];
			iSumLength+=iStick[i];
		}
		sort(iStick,iStick+iStickNum,GREATER);
		bTemp=false;
		for(iOriginLen=iStick[0];iOriginLen<=iSumLength/2;iOriginLen++)
			if(!(iSumLength%iOriginLen))
			{
				for(i=0;i<iStickNum;i++)
					bStick[i]=false;
				bStick[0]=true;
				iOriginNum=iSumLength/iOriginLen;
				bTemp=CreateStick(1,iStick[0],1);
				if(bTemp)
					break;
			}
		cout<<(bTemp?iOriginLen:iSumLength)<<endl;
	}
}

int GREATER(int iLeft,int iRight)
{
	return iLeft>iRight;
}

bool CreateStick(int iCode,int iLength,int iStart)
{
	int i;
	bool bTemp;
	if(iLength==iOriginLen)
	{
		if(iCode==iOriginNum)
			return true;
		for(i=0;;i++)
			if(!bStick[i])
			{
				bStick[i]=true;
				break;
			}
		bTemp=CreateStick(iCode+1,iStick[i],i+1);
		bStick[i]=false;
		return bTemp;
	}
	for(i=iStart;i<iStickNum;i++)
		if(!bStick[i])
		{
			if(iStick[i]+iLength==iOriginLen)
			{
				bStick[i]=true;
				bTemp=CreateStick(iCode,iOriginLen,iStart+1);
				bStick[i]=false;
				return bTemp;
			}
			else if(iStick[i]+iLength<iOriginLen)
			{
				bStick[i]=true;
				bTemp=CreateStick(iCode,iStick[i]+iLength,iStart+1);
				bStick[i]=false;
				if(bTemp)
					return bTemp;
			}
		}
	return false;
}
