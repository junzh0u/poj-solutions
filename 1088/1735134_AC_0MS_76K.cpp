#include<iostream.h>

#define LIMIT 100

short iHeight[LIMIT][LIMIT],iRow,iCol,iLength[LIMIT][LIMIT];

short NextSect(short,short);
short Max(short,short);

void main()
{
	short i,j,iMaxLength;
	cin>>iRow>>iCol;
	for(i=0;i<iRow;i++)
		for(j=0;j<iCol;j++)
			cin>>iHeight[i][j];
	iMaxLength=0;
	for(i=0;i<iRow;i++)
		for(j=0;j<iCol;j++)
			iLength[i][j]=0;
	for(i=0;i<iRow && iMaxLength!=iRow*iCol;i++)
		for(j=0;j<iCol && iMaxLength!=iRow*iCol;j++)
		{
			iLength[i][j]=NextSect(i,j);
			if(iLength[i][j]>iMaxLength)
				iMaxLength=iLength[i][j];
		}
	cout<<iMaxLength<<endl;
}

short NextSect(short iX,short iY)
{
	if(iLength[iX][iY]==0)
	{
		iLength[iX][iY]++;
		if(iX>0 && iHeight[iX-1][iY]>iHeight[iX][iY])
		{
			if(iLength[iX-1][iY])
				iLength[iX][iY]=Max(iLength[iX][iY],iLength[iX-1][iY]+1);
			else
				iLength[iX][iY]=Max(iLength[iX][iY],NextSect(iX-1,iY)+1);
		}
		if(iY>0 && iHeight[iX][iY-1]>iHeight[iX][iY])
		{
			if(iLength[iX][iY-1])
				iLength[iX][iY]=Max(iLength[iX][iY],iLength[iX][iY-1]+1);
			else
				iLength[iX][iY]=Max(iLength[iX][iY],NextSect(iX,iY-1)+1);
		}
		if(iX<iRow-1 && iHeight[iX+1][iY]>iHeight[iX][iY])
		{
			if(iLength[iX+1][iY])
				iLength[iX][iY]=Max(iLength[iX][iY],iLength[iX+1][iY]+1);
			else
				iLength[iX][iY]=Max(iLength[iX][iY],NextSect(iX+1,iY)+1);
		}
		if(iY<iCol-1 && iHeight[iX][iY+1]>iHeight[iX][iY])
		{
			if(iLength[iX][iY+1])
				iLength[iX][iY]=Max(iLength[iX][iY],iLength[iX][iY+1]+1);
			else
				iLength[iX][iY]=Max(iLength[iX][iY],NextSect(iX,iY+1)+1);
		}
	}
	return iLength[iX][iY];
}

short Max(short iLeft,short iRight)
{
	return ((iLeft>iRight)?iLeft:iRight);
}
