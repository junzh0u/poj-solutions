#include<iostream.h>

/*void printTri(int iRowNum,int iTri[100][100])
{
	int i,j;
	cout<<"\n";
	for(i=0;i<iRowNum;i++)
	{
		for(j=0;j<=i;j++)
			cout<<iTri[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"\n";
}*/

void main()
{
	int iRowNum,iTri[100][100],i,j;
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
			iTri[i][j]=0;
	}
	cin>>iRowNum;
	//printTri(iRowNum,iTri);
	for(i=0;i<iRowNum;i++)
	{
		for(j=0;j<=i;j++)
			cin>>iTri[i][j];
	}
	//printTri(iRowNum,iTri);
	while(iRowNum>1)
	{
		for(i=0;i<=iRowNum-2;i++)
		{
			if(iTri[iRowNum-1][i]>=iTri[iRowNum-1][i+1])
				iTri[iRowNum-2][i]+=iTri[iRowNum-1][i];
			else
				iTri[iRowNum-2][i]+=iTri[iRowNum-1][i+1];
		}
		iRowNum--;
		//printTri(iRowNum,iTri);
	}
	cout<<iTri[0][0]<<endl;
}