#include<iostream.h>

void Move(int iRow,int iCol,char cMap[][21],const int iWidth,const int iHeight)
{
	cMap[iRow][iCol]='@';
	if(iRow>0 && cMap[iRow-1][iCol]=='.')
		Move(iRow-1,iCol,cMap,iWidth,iHeight);
	if(iCol>0 && cMap[iRow][iCol-1]=='.')
		Move(iRow,iCol-1,cMap,iWidth,iHeight);
	if(iRow<iHeight-1 && cMap[iRow+1][iCol]=='.')
		Move(iRow+1,iCol,cMap,iWidth,iHeight);
	if(iCol<iWidth-1 && cMap[iRow][iCol+1]=='.')
		Move(iRow,iCol+1,cMap,iWidth,iHeight);
}

void main()
{
	int iWidth,iHeight,i,j,iCount;
	char cMap[20][21];
	while(cin>>iWidth>>iHeight)
	{
		if(!iWidth)
			break;
		for(i=0;i<iHeight;i++)
			cin>>cMap[i];
		for(i=0;i<iHeight;i++)
			for(j=0;j<iWidth;j++)
				if(cMap[i][j]=='@')
				{
					Move(i,j,cMap,iWidth,iHeight);
					break;
				}
		iCount=0;
		for(i=0;i<iHeight;i++)
			for(j=0;j<iWidth;j++)
				if(cMap[i][j]=='@')
					iCount++;
		cout<<iCount<<endl;
	}
}