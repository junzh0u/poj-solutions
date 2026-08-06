#include<iostream.h>

int Visit(int iMap[][50],int iRow,int iCol)
{
	int iRet=1,iTemp;
	iTemp=iMap[iRow][iCol];
	iMap[iRow][iCol]=-1;
	if(iTemp%2==0 && iMap[iRow][iCol-1]!=-1)
		iRet+=Visit(iMap,iRow,iCol-1);
	if((iTemp/2)%2==0 && iMap[iRow-1][iCol]!=-1)
		iRet+=Visit(iMap,iRow-1,iCol);
	if((iTemp/4)%2==0 && iMap[iRow][iCol+1]!=-1)
		iRet+=Visit(iMap,iRow,iCol+1);
	if((iTemp/8)%2==0 && iMap[iRow+1][iCol]!=-1)
		iRet+=Visit(iMap,iRow+1,iCol);
	return iRet;
}

void main()
{
	int iTemp,iRowMax,iColMax,iRoomNum,i,j,iMax;
	int iMap[50][50];
	cin>>iRowMax>>iColMax;
	for(i=0;i<iRowMax;i++)
		for(j=0;j<iColMax;j++)
			cin>>iMap[i][j];
	iRoomNum=iMax=0;
	for(i=0;i<iRowMax;i++)
		for(j=0;j<iColMax;j++)
			if(iMap[i][j]!=-1)
			{
				iTemp=Visit(iMap,i,j);
				if(iTemp>iMax)
					iMax=iTemp;
				iRoomNum++;
			}
	cout<<iRoomNum<<"\n"<<iMax<<endl;
}