#include<stdio.h>

int Min(int iLeft,int iRight)
{
	return (iLeft<iRight)?iLeft:iRight;
}
int main()
{
	int iScroll[3][5000],iScrPtr=0,i,iLength;
	char cString[5100];
	scanf("%d\n%s",&iLength,cString);
	for(iScrPtr=0;iScrPtr<iLength;iScrPtr++)
	{
		for(i=0;i<iLength-iScrPtr;i++)
		{
			if(!iScrPtr)
				iScroll[iScrPtr%3][i]=0;
			else if(iScrPtr==1)
				iScroll[iScrPtr%3][i]=(cString[i]==cString[i+iScrPtr])?0:1;
			else
				iScroll[iScrPtr%3][i]=(cString[i]==cString[i+iScrPtr])?iScroll[(iScrPtr+1)%3][i+1]:(Min(iScroll[(iScrPtr+2)%3][i],iScroll[(iScrPtr+2)%3][i+1])+1);
		}
	}
	printf("%d\n",iScroll[(iLength-1)%3][0]);
	return 0;
}