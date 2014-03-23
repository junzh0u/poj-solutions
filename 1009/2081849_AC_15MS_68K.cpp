#include<stdio.h>
#include<math.h>

int iWidth,iRle[1100][2],iRlePairNum,iCurrentPair[3],iLeftPixelNum[3],iColumn;

int RunAfter()
{
	int iMinSequenceAfter=2000000000,i;
	for(i=0;i<3;i++)
	{
		if(iLeftPixelNum[i]==iRle[iCurrentPair[i]][1]-1)
			return 1;
		if(iMinSequenceAfter>iLeftPixelNum[i])
			iMinSequenceAfter=iLeftPixelNum[i];
	}
	if(iMinSequenceAfter==0)
		return 1;
	return iMinSequenceAfter;
}

int EdgeDetect()
{
	int iCurrentPixel=iRle[iCurrentPair[1]][0],iMaxDifference=0,i;
	for(i=0;i<3;i++)
		if(iCurrentPair[i]>0 && iCurrentPair[i]<=iRlePairNum)
		{
			iMaxDifference=(fabs(iRle[iCurrentPair[i]][0]-iCurrentPixel)>iMaxDifference)?fabs(iRle[iCurrentPair[i]][0]-iCurrentPixel):iMaxDifference;
			if(iColumn>0 && iLeftPixelNum[i]==iRle[iCurrentPair[i]][1]-1)
				iMaxDifference=(fabs(iRle[iCurrentPair[i]-1][0]-iCurrentPixel)>iMaxDifference)?fabs(iRle[iCurrentPair[i]-1][0]-iCurrentPixel):iMaxDifference;
			if(iColumn<iWidth-1 && iLeftPixelNum[i]==0)
				iMaxDifference=(fabs(iRle[iCurrentPair[i]+1][0]-iCurrentPixel)>iMaxDifference)?fabs(iRle[iCurrentPair[i]+1][0]-iCurrentPixel):iMaxDifference;
		}
	return iMaxDifference;
}

void Advance(const int iSpan)
{
	int i,iSpanTemp;
	iColumn=(iColumn+iSpan)%iWidth;
	for(i=0;i<3;i++)
	{
		iSpanTemp=iSpan;
		while(iSpanTemp>0)
		{
			if(iSpanTemp<=iLeftPixelNum[i])
			{
				iLeftPixelNum[i]-=iSpanTemp;
				break;
			}
			else
			{
				iSpanTemp-=(iLeftPixelNum[i]+1);
				iCurrentPair[i]++;
				iLeftPixelNum[i]=iRle[iCurrentPair[i]][1]-1;
			}
		}
	}
}

void main()
{
	int i,iOutRleBuffer[2][2];
	while(scanf("%d",&iWidth))
	{
		printf("%d\n",iWidth);
		if(iWidth==0)
			break;
		iRlePairNum=0;
		iRle[0][0]=0;iRle[0][1]=2*iWidth;
		while(true)
		{
			scanf("%d %d",&iRle[iRlePairNum+1][0],&iRle[iRlePairNum+1][1]);
			if(iRle[iRlePairNum+1][0]==0 && iRle[iRlePairNum+1][1]==0)
				break;
			iRlePairNum++;
		}
		iRle[iRlePairNum+1][1]=2*iWidth;
		iColumn=0;
		for(i=0;i<3;i++)
		{
			iCurrentPair[i]=0;
			iLeftPixelNum[i]=(3-i)*iWidth-1;
		}
		Advance(2*iWidth);
		iOutRleBuffer[0][0]=EdgeDetect();iOutRleBuffer[0][1]=RunAfter();
		Advance(iOutRleBuffer[0][1]);
		while(iCurrentPair[1]<=iRlePairNum)
		{
			iOutRleBuffer[1][0]=EdgeDetect();iOutRleBuffer[1][1]=RunAfter();
			Advance(iOutRleBuffer[1][1]);
			if(iOutRleBuffer[0][0]==iOutRleBuffer[1][0])
				iOutRleBuffer[0][1]+=iOutRleBuffer[1][1];
			else
			{
				printf("%d %d\n",iOutRleBuffer[0][0],iOutRleBuffer[0][1]);
				iOutRleBuffer[0][0]=iOutRleBuffer[1][0];
				iOutRleBuffer[0][1]=iOutRleBuffer[1][1];
			}
		}
		printf("%d %d\n0 0\n",iOutRleBuffer[0][0],iOutRleBuffer[0][1]);
	}
}