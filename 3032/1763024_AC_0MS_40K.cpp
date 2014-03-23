#include<iostream.h>

void main()
{
	int iCaseNum,iCardNum,iSequence[13],i,j,iPoint;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iCardNum;
		for(i=0;i<iCardNum;i++)
			iSequence[i]=0;
		iPoint=0;
		for(i=1;i<=iCardNum;i++)
		{
			for(j=1;j<=i;j++)
			{
				iPoint=(iPoint+1)%iCardNum;
				while(iSequence[iPoint])
					iPoint=(iPoint+1)%iCardNum;
			}
			iSequence[iPoint]=i;
			if(i!=iCardNum)
				while(iSequence[iPoint])
					iPoint=(iPoint+1)%iCardNum;
		}
		for(i=0;i<iCardNum;i++)
			cout<<iSequence[i]<<" ";
		cout<<endl;
	}
}