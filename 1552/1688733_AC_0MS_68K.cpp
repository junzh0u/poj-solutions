#include<iostream.h>

void main()
{
	int iArray[16],iCount,iDouble,i,j;
	while(1)
	{
		cin>>iArray[0];
		if(iArray[0]==-1)
			break;
		iCount=1;
		while(cin>>iArray[iCount])
		{
			if(!iArray[iCount])
				break;
			iCount++;
		}
		iDouble=0;
		for(i=0;i<iCount;i++)
			for(j=i+1;j<iCount;j++)
				if(iArray[j]==2*iArray[i] || iArray[i]==iArray[j]*2)
					iDouble++;
		cout<<iDouble<<endl;
	}
}