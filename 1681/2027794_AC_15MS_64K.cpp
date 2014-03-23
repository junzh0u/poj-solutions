#include<iostream.h>
#include<string.h>

void print(int a,int size)
{
	int i;
	for(i=size;i>=0;i--)
		cout<<((a>>i)&1)<<" ";
}

void main()
{
	int iCaseNum,iSize,iMap[20],iTemp[20],iOper[20],i,j,k,iMin,iCount;
	char cTemp;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iSize;
		memset(iMap,0,sizeof(iMap));
		for(i=0;i<iSize;i++)
		{
			for(j=0;j<iSize;j++)
			{
				cin>>cTemp;
				if(cTemp=='w')
					iMap[i]|=1<<j;
			}
		}
		iMin=300;
		for(i=0;i<(1<<iSize);i++)
		{
			memcpy(iTemp,iMap,sizeof(iMap));
			for(j=0;j<iSize;j++)
			{
				if(j==0)
					iOper[j]=i;
				else
					iOper[j]=iTemp[j-1];
				iTemp[j]=((iTemp[j]^iOper[j])^((iOper[j]<<1)&((1<<iSize)-1)) )^(iOper[j]>>1);
				iTemp[j+1]^=iOper[j];
			}
			if(iTemp[iSize-1]==0)
			{
				iCount=0;
				for(j=0;j<iSize;j++)
					for(k=iOper[j];k;k>>=1)
						if(k&1)
							iCount++;
				if(iCount<iMin)
					iMin=iCount;
			}
		}
		if(iMin==300)
			cout<<"inf"<<endl;
		else
			cout<<iMin<<endl;
	}
}