#include<stdio.h>
#include<string.h>
#include<math.h>

#define TEST 0

int gcd(int a,int b)
{
	if(b==0)
		return a;
	return gcd(b,a%b);
}

int lcm(int a,int b)
{
	if(a>b)
		return a*b/gcd(a,b);
	else if(a<b)
		return a*b/gcd(b,a);
	else
		return a;
}

void mul(int* iArray,int iMul,const int iSize)
{
	int i;
	for(i=0;i<iSize;i++)
		iArray[i]*=iMul;
}

void swap(int* iLeft,int* iRight,const int iSize)
{
	int iTemp[100];
	memcpy(iTemp,iLeft,sizeof(int)*iSize);
	memcpy(iLeft,iRight,sizeof(int)*iSize);
	memcpy(iRight,iTemp,sizeof(int)*iSize);
}

void min(int* iLeft,const int *iRight,int iMul,const int iSize)
{
	int i;
	for(i=0;i<iSize;i++)
		iLeft[i]-=iRight[i]*iMul;
}

void main()
{
	const int iCoef[9][9]={	{1,1,0,1,0,0,0,0,0},
							{1,1,1,0,1,0,0,0,0},
							{0,1,1,0,0,1,0,0,0},
							{1,0,0,1,1,0,1,0,0},
							{1,0,1,0,1,0,1,0,1},
							{0,0,1,0,1,1,0,0,1},
							{0,0,0,1,0,0,1,1,0},
							{0,0,0,0,1,0,1,1,1},
							{0,0,0,0,0,1,0,1,1}};
	int iMatrix[9][10],i,j,k,iLine;
			
	for(i=0;i<9;i++)
	{
		memcpy(iMatrix[i],iCoef[i],sizeof(iCoef[i]));
		scanf("%d",&iMatrix[i][9]);
		iMatrix[i][9]=4-iMatrix[i][9];
	}
	iLine=0;
	for(i=0;i<9;i++)
	{
		for(j=iLine;j<9;j++)
			if(iMatrix[j][i])
			{
				if(iMatrix[j][i]<0)
					mul(iMatrix[j],-1,10);
				break;
			}
		if(j==9)
			continue;
		if(j!=iLine)
			swap(iMatrix[iLine],iMatrix[j],10);
		for(j=0;j<9;j++)
		{
			if(j==iLine || iMatrix[j][i]==0)
				continue;
			mul(iMatrix[j],fabs(lcm(iMatrix[j][i],iMatrix[iLine][i])/iMatrix[j][i]),10);
			min(iMatrix[j],iMatrix[iLine],iMatrix[j][i]/iMatrix[iLine][i],10);
		}
		iLine++;
	}
	for(i=0;i<9;i++)
	{
		for(j=0;j<4;j++)
			if(j*iMatrix[i][i]%4==(iMatrix[i][9]%4+4)%4)
				break;
		for(k=0;k<j;k++)
			printf("%d ",i+1);
	}
	printf("\n");
}