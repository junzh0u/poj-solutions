#include<iostream.h>
#include<math.h>

struct FourthOrderMatrix{int iMatrix[4][4];};

void Print(FourthOrderMatrix fomBuffer)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
			cout<<fomBuffer.iMatrix[i][j]<<" ";
		cout<<endl;
	}
}

FourthOrderMatrix ModMultiply(FourthOrderMatrix fomLeft,FourthOrderMatrix fomRight)
{
	FourthOrderMatrix fomReturn;
	int i,j,k;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			fomReturn.iMatrix[i][j]=0;
			for(k=0;k<4;k++)
				fomReturn.iMatrix[i][j]+=fomLeft.iMatrix[i][k]*fomRight.iMatrix[k][j];
			fomReturn.iMatrix[i][j]%=2005;
		}
	return fomReturn;
}

void main()
{
	FourthOrderMatrix fomPretreat[27]={{{{1,0,1,1},{1,0,0,0},{0,1,0,0},{0,0,1,0}}}};
	int i,iLength;
	const int iFirstFour[4]={2,4,6,9};
	for(i=1;i<27;i++)
		fomPretreat[i]=ModMultiply(fomPretreat[i-1],fomPretreat[i-1]);
	while(cin>>iLength)
	{
		if(iLength<=4)
		{
			cout<<iFirstFour[iLength-1]<<endl;
			continue;
		}
		iLength-=4;
		FourthOrderMatrix fomCoefficient={{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};
		for(i=26;i>=0;i--)
			if(iLength>=pow(2,i))
			{
				fomCoefficient=ModMultiply(fomCoefficient,fomPretreat[i]);
				iLength-=pow(2,i);
			}
		cout<<(fomCoefficient.iMatrix[0][0]*iFirstFour[3]+fomCoefficient.iMatrix[0][1]*iFirstFour[2]+fomCoefficient.iMatrix[0][2]*iFirstFour[1]+fomCoefficient.iMatrix[0][3]*iFirstFour[0])%2005<<endl;
	}
}