#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 20

struct Energy{double dCalory,dPercent;} eFood[5];

int Round(double dValue){return int(dValue)+((dValue-int(dValue)-0.5>0-1e-12)?1.0:0.0);}

void main()
{
	const double CONTAIN[5]={9.0,4.0,4.0,4.0,7.0};
	char cLine[5*MAX],cFood[5][MAX];int i;
	double dUsePercent,dTotal,dFatSum=0.0,dTotalSum=0.0;
	while(gets(cLine))
	{
		if(strcmp(cLine,"-")==0)
		{
			if(dTotalSum<1e-12)break;
			printf("%d%%\n",Round(dFatSum*100.0/dTotalSum));dFatSum=dTotalSum=0.0;
			continue;
		}
		sscanf(cLine,"%s%s%s%s%s",cFood[0],cFood[1],cFood[2],cFood[3],cFood[4]);
		memset(eFood,0,sizeof(eFood));dUsePercent=1.0;dTotal=0.0;
		for(i=0;i<5;i++)
			switch(cFood[i][strlen(cFood[i])-1])
			{
			case'C':dTotal+=(eFood[i].dCalory=atoi(cFood[i]));break;
			case'g':dTotal+=(eFood[i].dCalory=atoi(cFood[i])*CONTAIN[i]);break;
			case'%':dUsePercent-=(eFood[i].dPercent=atoi(cFood[i])/100.0);break;
			}
		if(dUsePercent<1.0)
		{
			dTotal/=dUsePercent;
			if(eFood[0].dPercent>0.0)eFood[0].dCalory=dTotal*eFood[0].dPercent;
		}
		dTotalSum+=dTotal;dFatSum+=eFood[0].dCalory;
	}
}