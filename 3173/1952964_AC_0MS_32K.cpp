#include<iostream.h>

void main()
{
	int iMatrix[20][20],i,j,iSize,iSeed;
	cin>>iSize>>iSeed;
	iMatrix[0][0]=iSeed;
	for(i=1;i<iSize;i++)
		iMatrix[0][i]=(iMatrix[0][i-1]+i-1)%9+1;
	for(i=1;i<iSize;i++)
		for(j=0;j<iSize;j++)
			if(j<i)
				iMatrix[i][j]=0;
			else
				iMatrix[i][j]=iMatrix[i-1][j]%9+1;
	for(i=0;i<iSize;i++)
	{
		for(j=0;j<iSize;j++)
			cout<<char(iMatrix[i][j]?iMatrix[i][j]+'0':' ')<<" ";
		cout<<endl;
	}
}