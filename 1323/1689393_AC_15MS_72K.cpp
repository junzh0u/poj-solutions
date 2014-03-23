#include<iostream.h>

void main()
{
	int iAllCard[1000],iPlayerCard[50],iPlayerNum,iCardNum,iWinSum,iCount=0,i,j;
	while(1)
	{
		cin>>iPlayerNum>>iCardNum;
		if(!iPlayerNum)
			break;
		iCount++;
		for(i=0;i<iPlayerNum*iCardNum;i++)
			iAllCard[i]=1;
		for(i=0;i<iCardNum;i++)
		{
			cin>>iPlayerCard[i];
			iAllCard[iPlayerCard[i]-1]=0;
		}
		iWinSum=0;
		for(i=0;i<iCardNum;i++)
		{
			if(iPlayerCard[i]==iPlayerNum*iCardNum)
				iWinSum++;
			else
			{
				for(j=iPlayerCard[i];j<iPlayerNum*iCardNum;j++)
					if(iAllCard[j])
					{
						iAllCard[j]=0;
						break;
					}
				if(j==iPlayerNum*iCardNum)
					iWinSum++;
			}
		}
		cout<<"Case "<<iCount<<": "<<iWinSum<<endl;
	}
}