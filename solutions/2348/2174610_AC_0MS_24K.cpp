#include<stdio.h>

void Swap(__int64& iLeft,__int64& iRight){__int64 iTemp=iLeft;iLeft=iRight;iRight=iTemp;}

void main()
{
	__int64 iGreater,iLesser;bool bStan;
	while(scanf("%I64d%I64d",&iGreater,&iLesser) && iGreater!=0)
	{
		if(iGreater<iLesser)Swap(iGreater,iLesser);
		//printf("%I64d %I64d\n",iGreater,iLesser);
		bStan=true;
		while(iGreater>iLesser && iGreater<iLesser*2)
		{
			iGreater-=iLesser;
			Swap(iGreater,iLesser);
			//printf("%I64d %I64d\n",iGreater,iLesser);
			bStan=!bStan;
		}
		printf("%s wins\n",bStan?"Stan":"Ollie");
	}
}