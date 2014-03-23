#include<stdio.h>
#include<string.h>
#include<stdlib.h>

const char WEEKDAY[7][5]={"MON","TUE","WED","THU","FRI","SAT","SUN"};

struct Equation{int iCoef[300],iConst;};
Equation eEquas[300];
int iTypeNum,iRecordNum;

int WeekDay(char* cString)
{for(int iDay=0;iDay<7;iDay++)if(strcmp(WEEKDAY[iDay],cString)==0)return iDay+1;return -1;}

int gcd(int a,int b){if(b==0)return a;return gcd(b,a%b);}

int lcm(int a,int b){if(a>b)return a*b/gcd(a,b);else if(a<b)return a*b/gcd(b,a);else return a;}

void Mod(Equation* peEqua){for(int i=0;i<iTypeNum;i++)peEqua->iCoef[i]%=7;peEqua->iConst%=7;}

void Multiply(Equation* peEqua,int iMul)
{for(int i=0;i<iTypeNum;i++)peEqua->iCoef[i]*=iMul;peEqua->iConst*=iMul;}

void Swap(Equation* peLeft,Equation* peRight)
{Equation eTemp;eTemp=*peLeft;*peLeft=*peRight;*peRight=eTemp;}

void Minus(Equation* peLeft,Equation* peRight,int iMul)
{for(int i=0;i<iTypeNum;i++)peLeft->iCoef[i]-=peRight->iCoef[i]*iMul;peLeft->iConst-=peRight->iConst*iMul;}

void Simplify()
{
	int iSymbol,iLine=0,iCurLine;
	for(iSymbol=0;iSymbol<iTypeNum;iSymbol++)
	{
		for(iCurLine=iLine;iCurLine<iRecordNum;iCurLine++)
			if(eEquas[iCurLine].iCoef[iSymbol]!=0)
			{
				if(eEquas[iCurLine].iCoef[iSymbol]<0)
					Multiply(&eEquas[iCurLine],-1);
				break;
			}
		if(iCurLine==iRecordNum)continue;
		if(iCurLine!=iLine)Swap(&eEquas[iCurLine],&eEquas[iLine]);
		for(iCurLine=0;iCurLine<iRecordNum;iCurLine++)
		{
			if(iCurLine==iLine || eEquas[iCurLine].iCoef[iSymbol]==0)continue;
			if(eEquas[iCurLine].iCoef[iSymbol]<0)Multiply(&eEquas[iCurLine],-1);
			Multiply(&eEquas[iCurLine],lcm(eEquas[iCurLine].iCoef[iSymbol],eEquas[iLine].iCoef[iSymbol])/eEquas[iCurLine].iCoef[iSymbol]);
			Minus(&eEquas[iCurLine],&eEquas[iLine],eEquas[iCurLine].iCoef[iSymbol]/eEquas[iLine].iCoef[iSymbol]);
			Mod(&eEquas[iCurLine]);Mod(&eEquas[iLine]);
		}
		iLine++;
	}
}

bool NotZero(Equation* peEqua)
{for(int i=0;i<iTypeNum;i++)if(peEqua->iCoef[i]!=0)return true;return false;}

int Judge()
{
	int i,iNotZeroCount=0;
	for(i=0;i<iRecordNum;i++)
	{
		if(NotZero(&eEquas[i]))iNotZeroCount++;
		else if(eEquas[i].iConst!=0)return 0;
	}
	if(iNotZeroCount==iTypeNum)return 1;
	else if(iNotZeroCount<iTypeNum)return 2;
	else return 0;
}

void main()
{
	char cBegin[5],cEnd[5];
	int i,j,iWidgetNum,iType,iJudge,iLine,iDay;
	while(scanf("%d%d",&iTypeNum,&iRecordNum))
	{
		if(iTypeNum==0)break;
		for(i=0;i<iRecordNum;i++)
		{
			scanf("%d%s%s",&iWidgetNum,cBegin,cEnd);
			eEquas[i].iConst=WeekDay(cEnd)-WeekDay(cBegin)+1;
			memset(eEquas[i].iCoef,0,sizeof(int)*iTypeNum);
			for(j=0;j<iWidgetNum;j++)
			{
				scanf("%d",&iType);
				eEquas[i].iCoef[iType-1]++;
			}
			Mod(&eEquas[i]);
		}
		Simplify();
		iJudge=Judge();
		if(iJudge==0)printf("Inconsistent data.\n");
		else if(iJudge==2)printf("Multiple solutions.\n");
		else
		{
			for(i=0;i<iTypeNum;i++)
			{
				for(iLine=0;iLine<iRecordNum && eEquas[iLine].iCoef[i]==0;iLine++);
				for(iDay=3;iDay<=9;iDay++)
					if((eEquas[iLine].iCoef[i]*iDay-eEquas[iLine].iConst)%7==0)
						break;
				printf("%d",iDay);
				if(i!=iTypeNum-1)
					printf(" ");
			}
			printf("\n");
		}
	}
}