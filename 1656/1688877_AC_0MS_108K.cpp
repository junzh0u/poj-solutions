#include<iostream.h>
#include<string.h>

void main()
{
	int iBoard[100][100],i,j,k,iX,iY,iL,iComNum,iCount;
	char cCommand[6];
	cin>>iComNum;
	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
			iBoard[i][j]=0;
	for(i=0;i<iComNum;i++)
	{
		cin>>cCommand>>iX>>iY>>iL;
		if(!strcmp(cCommand,"WHITE"))
			for(j=iX-1;j<iX+iL-1;j++)
				for(k=iY-1;k<iY+iL-1;k++)
					iBoard[j][k]=0;
		else if(!strcmp(cCommand,"BLACK"))
			for(j=iX-1;j<iX+iL-1;j++)
				for(k=iY-1;k<iY+iL-1;k++)
					iBoard[j][k]=1;
		else if(!strcmp(cCommand,"TEST"))
		{
			iCount=0;
			for(j=iX-1;j<iX+iL-1;j++)
				for(k=iY-1;k<iY+iL-1;k++)
					if(iBoard[j][k]==1)
						iCount++;
			cout<<iCount<<endl;
		}
	}
}