#include<iostream.h>
#include<string.h>

void Go(char cMap[][33],int* iRow,int* iCol,char cDir)
{
	if(cDir=='W')
	{
		cMap[*iRow][*iCol-1]='X';
		(*iCol)--;
	}
	else if(cDir=='E')
	{
		cMap[*iRow-1][*iCol]='X';
		(*iCol)++;
	}
	else if(cDir=='N')
	{
		cMap[*iRow][*iCol]='X';
		(*iRow)++;
	}
	else if(cDir=='S')
	{
		cMap[*iRow-1][*iCol-1]='X';
		(*iRow)--;
	}
}

void main()
{
	char cMap[32][33],cTemp[1000];
	int iCaseNum,iCol,iRow,i,j,k;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>iCol>>iRow;
		cin>>cTemp;
		for(j=0;j<32;j++)
		{
			for(k=0;k<32;k++)
				cMap[j][k]='.';
			cMap[j][32]='\0';
		}
		for(j=0;cTemp[j]!='.';j++)
			Go(cMap,&iRow,&iCol,cTemp[j]);
		cout<<"Bitmap #"<<i<<endl;
		for(j=31;j>=0;j--)
			cout<<cMap[j]<<endl;
		cout<<endl;
	}
}