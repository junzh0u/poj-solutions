#include<iostream.h>

void main()
{
	int iLine,iMatrix[350][350],i,j;
	cin>>iLine;
	for(i=0;i<iLine;i++)
		for(j=0;j<=i;j++)
			cin>>iMatrix[i][j];
	for(i=iLine-2;i>=0;i--)
		for(j=0;j<=i;j++)
			iMatrix[i][j]+=iMatrix[i+1][j]>iMatrix[i+1][j+1]?iMatrix[i+1][j]:iMatrix[i+1][j+1];
	cout<<iMatrix[0][0]<<endl;
}