#include<iostream.h>
#include<string.h>

int Count(int iMap[][100],int iX,int iY,const int iSize)
{
	int iSign=iMap[iX][iY],iRet=1;
	iMap[iX][iY]=-1;
	if(iX>0 && iMap[iX-1][iY]==iSign)
		iRet+=Count(iMap,iX-1,iY,iSize);
	if(iX<iSize-1 && iMap[iX+1][iY]==iSign)
		iRet+=Count(iMap,iX+1,iY,iSize);
	if(iY>0 && iMap[iX][iY-1]==iSign)
		iRet+=Count(iMap,iX,iY-1,iSize);
	if(iY<iSize-1 && iMap[iX][iY+1]==iSign)
		iRet+=Count(iMap,iX,iY+1,iSize);
	return iRet;
}

void main()
{
	int iMap[100][100],iSize,i,j,k,l,iX,iY;
	bool bGood;
	while(cin>>iSize)
	{
		if(iSize==0)
			break;
		memset(iMap,0,sizeof(iMap));
		for(i=1;i<iSize;i++)
			for(j=0;j<iSize;j++)
			{
				cin>>iX>>iY;
				iMap[iX-1][iY-1]=i;
			}
		bGood=true;
		for(i=0;i<iSize;i++)
			for(j=0;j<iSize;j++)
				if(iMap[i][j]!=-1 && Count(iMap,i,j,iSize)!=iSize)
				{
					/*cout<<endl;
					for(k=0;k<iSize;k++)
					{
						for(l=0;l<iSize;l++)
							cout<<iMap[k][l]<<" ";
						cout<<endl;
					}
					cout<<i<<" "<<j<<endl;*/
					bGood=false;
					break;
				}
		cout<<(bGood?"good":"wrong")<<endl;
	}
}