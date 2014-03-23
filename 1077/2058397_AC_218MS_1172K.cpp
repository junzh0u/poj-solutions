#include<iostream.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

long Factorial(long iExp)
{
	long iRet=1,i;
	for(i=2;i<=iExp;i++)
		iRet*=i;
	return iRet;
}

long CalReverse(char cPermutation[],long iPosition)
{
	long i,iRet=0;
	for(i=0;i<iPosition;i++)
		if(cPermutation[i]>cPermutation[iPosition])
			iRet++;
	return iRet++;
}

long Hash(long iPermutation)
{
	char cPerm[10];
	_itoa(iPermutation,cPerm,10);
	long iRet=0,i;
	for(i=1;i<8;i++)
		iRet+=CalReverse(cPerm,i)*Factorial(i);
	iRet+=(9-iPermutation%10)*Factorial(8);
	return iRet;
}

long Left(long iState){return iState-1;}
long Right(long iState){return iState+1;}
long Up(long iState)
{
	long iEnd=iState%((int)pow(10,10-iState%10));
	long iHead=iState/(int)pow(10,10-iState%10);
	long iMid=iHead%1000;
	iHead-=iMid;
	iMid=(iMid%100)*10+iMid/100;
	return (iHead+iMid)*(int)pow(10,10-iState%10)+iEnd-3;
}
long Down(long iState)
{
	long iEnd=iState%((int)pow(10,7-iState%10));
	long iHead=iState/(int)pow(10,7-iState%10);
	long iMid=iHead%1000;
	iHead-=iMid;
	iMid=(iMid%10)*100+iMid/10;
	return (iHead+iMid)*(int)pow(10,7-iState%10)+iEnd+3;
}

int BFS(char cTable[][400000],long iStart,long iEnd)
{
	long *iLeftQueue,*iRightQueue,iNowPtr,iLeftEnd,iRightEnd;
	iLeftQueue=new long[200000];iRightQueue=new long[200000];
	memset(cTable,' ',sizeof(char)*800000);
	iNowPtr=0;iLeftEnd=iRightEnd=1;
	iLeftQueue[0]=iStart;iRightQueue[0]=iEnd;
	cTable[0][Hash(iStart)]='s';cTable[1][Hash(iEnd)]='e';
	while(1)
	{
		if(iLeftQueue[iNowPtr]%10>3 && cTable[0][Hash(Up(iLeftQueue[iNowPtr]))]==' ')
		{
			cTable[0][Hash(Up(iLeftQueue[iNowPtr]))]='u';
			if(cTable[1][Hash(Up(iLeftQueue[iNowPtr]))]!=' ')
				return Up(iLeftQueue[iNowPtr]);
			iLeftQueue[iLeftEnd++]=Up(iLeftQueue[iNowPtr]);
		}
		if(iLeftQueue[iNowPtr]%10<7 && cTable[0][Hash(Down(iLeftQueue[iNowPtr]))]==' ')
		{
			cTable[0][Hash(Down(iLeftQueue[iNowPtr]))]='d';
			if(cTable[1][Hash(Down(iLeftQueue[iNowPtr]))]!=' ')
				return Down(iLeftQueue[iNowPtr]);
			iLeftQueue[iLeftEnd++]=Down(iLeftQueue[iNowPtr]);
		}
		if((iLeftQueue[iNowPtr]%10)%3!=1 && cTable[0][Hash(Left(iLeftQueue[iNowPtr]))]==' ')
		{
			cTable[0][Hash(Left(iLeftQueue[iNowPtr]))]='l';
			if(cTable[1][Hash(Left(iLeftQueue[iNowPtr]))]!=' ')
				return Left(iLeftQueue[iNowPtr]);
			iLeftQueue[iLeftEnd++]=Left(iLeftQueue[iNowPtr]);
		}
		if((iLeftQueue[iNowPtr]%10)%3!=0 && cTable[0][Hash(Right(iLeftQueue[iNowPtr]))]==' ')
		{
			cTable[0][Hash(Right(iLeftQueue[iNowPtr]))]='r';
			if(cTable[1][Hash(Right(iLeftQueue[iNowPtr]))]!=' ')
				return Right(iLeftQueue[iNowPtr]);
			iLeftQueue[iLeftEnd++]=Right(iLeftQueue[iNowPtr]);
		}

		if(iRightQueue[iNowPtr]%10>3 && cTable[1][Hash(Up(iRightQueue[iNowPtr]))]==' ')
		{
			cTable[1][Hash(Up(iRightQueue[iNowPtr]))]='d';
			if(cTable[0][Hash(Up(iRightQueue[iNowPtr]))]!=' ')
				return Up(iRightQueue[iNowPtr]);
			iRightQueue[iRightEnd++]=Up(iRightQueue[iNowPtr]);
		}
		if(iRightQueue[iNowPtr]%10<7 && cTable[1][Hash(Down(iRightQueue[iNowPtr]))]==' ')
		{
			cTable[1][Hash(Down(iRightQueue[iNowPtr]))]='u';
			if(cTable[0][Hash(Down(iRightQueue[iNowPtr]))]!=' ')
				return Down(iRightQueue[iNowPtr]);
			iRightQueue[iRightEnd++]=Down(iRightQueue[iNowPtr]);
		}
		if((iRightQueue[iNowPtr]%10)%3!=1 && cTable[1][Hash(Left(iRightQueue[iNowPtr]))]==' ')
		{
			cTable[1][Hash(Left(iRightQueue[iNowPtr]))]='r';
			if(cTable[0][Hash(Left(iRightQueue[iNowPtr]))]!=' ')
				return Left(iRightQueue[iNowPtr]);
			iRightQueue[iRightEnd++]=Left(iRightQueue[iNowPtr]);
		}
		if((iRightQueue[iNowPtr]%10)%3!=0 && cTable[1][Hash(Right(iRightQueue[iNowPtr]))]==' ')
		{
			cTable[1][Hash(Right(iRightQueue[iNowPtr]))]='l';
			if(cTable[0][Hash(Right(iRightQueue[iNowPtr]))]!=' ')
				return Right(iRightQueue[iNowPtr]);
			iRightQueue[iRightEnd++]=Right(iRightQueue[iNowPtr]);
		}
		iNowPtr++;
	}
}

void main()
{
	const long END=123456789;
	long i,iPtr=0,iRevSum=0;
	char cTemp,cStart[10],cTable[2][400000],cBefore[200000];//362880
	cStart[9]='\0';
	for(i=1;i<=9;i++)
	{
		cin>>cTemp;
		if(cTemp=='x')
			cStart[8]=char('0'+i);
		else
			cStart[iPtr++]=cTemp;
	}
	for(i=0;i<8;i++)
		iRevSum+=CalReverse(cStart,i);
	if(iRevSum%2!=0)
	{
		cout<<"unsolvable"<<endl;
		return;
	}
	iPtr=0;
	for(i=BFS(cTable,atoi(cStart),END);i!=atoi(cStart);)
	{
		cBefore[iPtr++]=cTable[0][Hash(i)];
		switch(cBefore[iPtr-1])
		{
		case 'u':i=Down(i);break;
		case 'd':i=Up(i);break;
		case 'l':i=Right(i);break;
		case 'r':i=Left(i);break;
		default:break;
		}
	}
	for(i=iPtr;i--;i>=0)
		cout<<cBefore[i];
	for(i=BFS(cTable,atoi(cStart),END);i!=END;)
	{
		cout<<cTable[1][Hash(i)];
		switch(cTable[1][Hash(i)])
		{
		case 'u':i=Up(i);break;
		case 'd':i=Down(i);break;
		case 'l':i=Left(i);break;
		case 'r':i=Right(i);break;
		default:break;
		}
	}
	cout<<endl;
}