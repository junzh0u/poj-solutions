#include<iostream.h>

void main()
{
	int iRowNum=0,iScore[150][150]={0},iSum=0,iMaxScore=0,iMax=0,i,j;
	cin>>iRowNum;
	for(i=0;i<iRowNum;i++)
	{
		iSum=0;
		for(j=0;j<iRowNum;j++){cin>>iScore[i][j];if(iScore[i][j]==3)iSum++;}
		if(iSum>iMaxScore){iMaxScore=iSum;iMax=i+1;}
	}
	cout<<iMax<<endl;
}