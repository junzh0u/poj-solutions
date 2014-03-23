#include<stdio.h>
#include<string.h>

struct Team
{
	char cName[10];
	int iSolve,iTime,iPenalty[10],iRank;
	bool bDone[10];
};

bool Greater(const Team* ptLeft,const Team* ptRight)
{
	if(ptLeft->iSolve!=ptRight->iSolve)
		return ptLeft->iSolve>ptRight->iSolve;
	if(ptLeft->iTime!=ptRight->iTime)
		return ptLeft->iTime<ptRight->iTime;
	if(strcmp(ptLeft->cName,ptRight->cName))
		return strcmp(ptLeft->cName,ptRight->cName)<0;
	return false;
}

void main()
{
	int iCase,iTeam,iProblem,iSubmit,i,j,iSubPro,iSubTime;
	Team tList[20],tTemp;
	char cJudge[5],cSubName[10];
	scanf("%d",&iCase);
	while(iCase--)
	{
		memset(tList,0,sizeof(tList));
		scanf("%d",&iTeam);
		for(i=0;i<iTeam;i++)
		{
			scanf("%s",tList[i].cName);
			tList[i].iSolve=0;
		}
		scanf("%d %d",&iProblem,&iSubmit);
		while(iSubmit--)
		{
			scanf("%d %d %s %s",&iSubPro,&iSubTime,cJudge,cSubName);
			for(i=0;i<iTeam;i++)
				if(strcmp(tList[i].cName,cSubName)==0)
				{
					if(tList[i].bDone[iSubPro-1]==false)
					{
						if(strcmp(cJudge,"Yes")==0)
						{
							tList[i].iTime+=iSubTime+tList[i].iPenalty[iSubPro-1]*20;
							tList[i].bDone[iSubPro-1]=true;
							tList[i].iSolve++;
						}
						else
							tList[i].iPenalty[iSubPro-1]++;
					}
					break;
				}
		}
		for(i=0;i<iTeam;i++)
			for(j=i+1;j<iTeam;j++)
				if(!Greater(&tList[i],&tList[j]))
				{
					tTemp=tList[i];
					tList[i]=tList[j];
					tList[j]=tTemp;
				}
		tList[0].iRank=1;
		for(i=1;i<iTeam;i++)
		{
			if(tList[i].iSolve==tList[i-1].iSolve && tList[i].iTime==tList[i-1].iTime)
				tList[i].iRank=tList[i-1].iRank;
			else
				tList[i].iRank=i+1;
		}
		for(i=0;i<iTeam;i++)
			printf("%2d. %-8s %1d %4d\n",tList[i].iRank,tList[i].cName,tList[i].iSolve,tList[i].iTime);
		printf("\n");
	}
}