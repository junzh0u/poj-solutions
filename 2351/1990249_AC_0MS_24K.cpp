#include<stdio.h>
#include<string.h>

void main()
{
	const int iDiffList[2][32]={{0,0,1,1,0,1,1,2,2,3,3,4,-4,-3,-3,-2,-5,-4,-6,-5,-7,-6,-8,
		-7,-10,-9,-8,10,11,9,10,8},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30,-30,0,0,0,0,0,0,0,0,0,
		0,0,0,0,30,30,0}};
	const char cNameList[32][5]={"UTC","GMT","BST","IST","WET","WEST","CET","CEST","EET",
		"EEST","MSK","MSD","AST","ADT","NST","NDT","EST","EDT","CST","CDT","MST","MDT",
		"PST","PDT","HST","AKST","AKDT","AEST","AEDT","ACST","ACDT","AWST"};
	int iCaseNum,iHour,iMin,i;
	char cDenote[9],cSouZone[5],cDesZone[5];
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		if(scanf("%d:%d %s %s %s",&iHour,&iMin,cDenote,cSouZone,cDesZone))
			iHour=((iHour==12)?0:iHour)+((strcmp(cDenote,"a.m."))?12:0);
		else
		{
			scanf("%s %s %s",cDenote,cSouZone,cDesZone);
			iHour=strcmp(cDenote,"noon")?0:12;
			iMin=0;
		}
		for(i=0;strcmp(cSouZone,cNameList[i]);i++);
		iHour-=iDiffList[0][i];
		iMin-=iDiffList[1][i];
		for(i=0;strcmp(cDesZone,cNameList[i]);i++);
		iHour+=iDiffList[0][i];
		iMin+=iDiffList[1][i];
		iHour=(((iMin>=0 && iMin<60)?iHour:((iMin<0)?iHour-1:iHour+1))+24)%24;
		iMin=(iMin+60)%60;
		if(!iHour)
		{
			if(!iMin)
				printf("midnight\n");
			else
				printf("12:%02d a.m.\n",iMin);
		}
		else if(iHour==12)
		{
			if(!iMin)
				printf("noon\n");
			else
				printf("12:%02d p.m.\n",iMin);
		}
		else
			printf("%d:%02d %s\n",iHour%12,iMin,iHour<12?"a.m.":"p.m.");

	}
}