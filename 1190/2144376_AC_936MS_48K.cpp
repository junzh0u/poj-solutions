#include<stdio.h>
#define MAX 20
#define INF 1000000
#define UNEXIST -1

int iMinArea=INF,MinSide[MAX+1],MinVolumn[MAX+1];
inline void Initialize()
{
	MinSide[0]=MinVolumn[0]=0;
	for(int i=1;i<=MAX;i++)
	{MinSide[i]=MinSide[i-1]+2*i*i;MinVolumn[i]=MinVolumn[i-1]+i*i*i;}
}
inline int MaxVolumn(int iLayer,int iHeight,int iRadius)
{
	int iReturn=0,i;
	for(i=1;i<=iLayer;i++)iReturn+=(iHeight-i)*(iRadius-i)*(iRadius-i);
	return iReturn;
}

inline int Max(int iLeft,int iRight){return (iLeft>iRight)?iLeft:iRight;}
int SideArea(int iVolumn,int iHeight,int iRadius,int iLayer,int iNowArea)
{
	iNowArea+=2*iHeight*iRadius;iVolumn-=iHeight*iRadius*iRadius;iLayer--;
	if(iLayer==0)
	{
		if(iVolumn==0)
			return 2*iHeight*iRadius;
		else return UNEXIST;
	}
	if(iNowArea+MinSide[iLayer]>=iMinArea)return UNEXIST;
	if(MaxVolumn(iLayer,iHeight,iRadius)<iVolumn)return UNEXIST;
	int iNextHeight,iNextRadius,iNextArea,iNextMin=INF;
	for(iNextRadius=iLayer;iNextRadius<iRadius && iNextRadius*iNextRadius+MinVolumn[iLayer-1]<=iVolumn;iNextRadius++)
		for(iNextHeight=Max(iLayer,iVolumn/(iLayer*iNextRadius*iNextRadius));iNextHeight<iHeight && iNextHeight*iNextRadius*iNextRadius+MinVolumn[iLayer-1]<=iVolumn;iNextHeight++)
		{
			iNextArea=SideArea(iVolumn,iNextHeight,iNextRadius,iLayer,iNowArea);
			if(iNextArea!=UNEXIST && iNextMin>iNextArea)
				iNextMin=iNextArea;
		}
	if(iNextMin==INF)return UNEXIST;
	else
		return 2*iHeight*iRadius+iNextMin;
}

void main()
{
	int iVolumn,iLayer,iRadius,iHeight,iArea;
	Initialize();
	scanf("%d%d",&iVolumn,&iLayer);
	for(iRadius=iLayer;iRadius*iRadius+MinVolumn[iLayer-1]<=iVolumn;iRadius++)
		for(iHeight=Max(iLayer,iVolumn/(iLayer*iRadius*iRadius));iHeight*iRadius*iRadius+MinVolumn[iLayer-1]<=iVolumn;iHeight++)
		{
			iArea=SideArea(iVolumn,iHeight,iRadius,iLayer,iRadius*iRadius);
			if(iArea!=UNEXIST && iMinArea>iArea+iRadius*iRadius)
				iMinArea=iArea+iRadius*iRadius;
		}
	if(iMinArea==INF)iMinArea=0;
	printf("%d\n",iMinArea);
}