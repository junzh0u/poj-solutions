#include<iostream.h>

#define LIMIT 1000

int iTrainNum,iOutSeq[LIMIT];

bool Judge();

void main()
{
	int i;
	while(1)
	{
		cin>>iTrainNum;
		if(!iTrainNum)
			break;
		while(1)
		{
			cin>>iOutSeq[0];
			if(!iOutSeq[0])
				break;
			for(i=1;i<iTrainNum;i++)
				cin>>iOutSeq[i];
			if(Judge())
				cout<<"Yes"<<endl;
			else
				cout<<"No"<<endl;
		}
		cout<<endl;
	}
}

bool Judge()
{
	int iIn,iOut,iPtSta,iPtOut,iStation[LIMIT];
	iPtOut=iPtSta=0;
	iOut=iOutSeq[iPtOut];
	iIn=1;
	while(1)
	{
		if(iOut==iIn)
		{
			iPtOut++;
			if(iPtOut==iTrainNum)
				return true;
			iOut=iOutSeq[iPtOut];
			iIn++;
			if(iIn>iTrainNum)
				iIn=0;
		}
		else if(iPtSta && iStation[iPtSta-1]==iOut)
		{
			iPtOut++;
			if(iPtOut==iTrainNum)
				return true;
			iOut=iOutSeq[iPtOut];
			iPtSta--;
		}
		else if(iIn)
		{
			iStation[iPtSta]=iIn;
			iPtSta++;
			iIn++;
			if(iIn>iTrainNum)
				iIn=0;
		}
		else
			return false;
	}
}