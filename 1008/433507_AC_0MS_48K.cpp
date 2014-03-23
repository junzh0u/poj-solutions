#include<iostream.h>
#include<stdio.h>
#include<string.h>

char nameHaab[19][10]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol","chen",
"yax","zac","ceh","mac","kankin","muan","pax","koyab","cumhu","uayet"};

char nameTzolkin[20][10]={"imix","ik","akbal","kan","chicchan","cimi","manik",
"lamat","muluk","ok","chuen","eb","ben","ix","mem","cib","caban","eznab","canac",
"ahau"};

int main()
{
	int i,j,iDataNum,iDaysTotal;
	int iHabbDay,iHabbMonth,iHabbYear,iTzolkinDay,iTzolkinMonth,iTzolkinYear;
	char chHabbMonth[10];
	scanf("%d",&iDataNum);
	printf("%d\n",iDataNum);
	for(i=0;i<iDataNum;i++)
	{
		scanf("%d.%s%d",&iHabbDay,chHabbMonth,&iHabbYear);
		for(j=0;j<19;j++)
		{
			if(strcmp(chHabbMonth,nameHaab[j])==0)
				iHabbMonth=j;
		}
		iDaysTotal=iHabbYear*365+iHabbMonth*20+iHabbDay;
		iTzolkinYear=iDaysTotal/260;
		iDaysTotal%=260;
		iTzolkinDay=iDaysTotal%13+1;
		iTzolkinMonth=iDaysTotal%20;
		printf("%d %s %d\n",iTzolkinDay,nameTzolkin[iTzolkinMonth],iTzolkinYear);
	}
	return 0;
}