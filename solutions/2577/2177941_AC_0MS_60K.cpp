#include<stdio.h>
#include<stdlib.h>

void main()
{
	int iRam[1000]={0},iReg[10]={0},iCur=0,iCount=0,i=0,iDigit[3];
	while(scanf("%d",&iRam[i++])!=EOF);
	while(iRam[iCur]!=100)
	{
		iDigit[0]=iRam[iCur]/100;iDigit[1]=(iRam[iCur]/10)%10;iDigit[2]=iRam[iCur]%10;
		//printf("%d %d %d\n",iDigit[0],iDigit[1],iDigit[2]);
		iCount++;
		switch(iDigit[0])
		{
		case 2:{iReg[iDigit[1]]=iDigit[2];break;}
		case 3:{iReg[iDigit[1]]=(iReg[iDigit[1]]+iDigit[2])%1000;break;}
		case 4:{iReg[iDigit[1]]=(iReg[iDigit[1]]*iDigit[2])%1000;break;}
		case 5:{iReg[iDigit[1]]=iReg[iDigit[2]];break;}
		case 6:{iReg[iDigit[1]]=(iReg[iDigit[1]]+iReg[iDigit[2]])%1000;break;}
		case 7:{iReg[iDigit[1]]=(iReg[iDigit[1]]*iReg[iDigit[2]])%1000;break;}
		case 8:{iReg[iDigit[1]]=iRam[iReg[iDigit[2]]];break;}
		case 9:{iRam[iReg[iDigit[2]]]=iReg[iDigit[1]];break;}
		case 0:{if(iReg[iDigit[2]]!=0){iCur=iReg[iDigit[1]]-1;}break;}
		}
		iCur++;
	}
	printf("%d\n",iCount+1);
}