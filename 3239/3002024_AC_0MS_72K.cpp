#include<cstdio>

void main()
{
	int queenNum,i;
	while(scanf("%d",&queenNum))
	{
		if(queenNum==0)break;
		if(queenNum%6!=2 && queenNum%6!=3)
		{
			for(i=2;i<=queenNum;i+=2)printf("%d ",i);
			for(i=1;i<=queenNum;i+=2)printf("%d ",i);
		}
		else
		{
			if((queenNum/2)%2==0)
			{
				for(i=queenNum/2;i<=queenNum;i+=2)printf("%d ",i);
				for(i=2;i<=queenNum/2-2;i+=2)printf("%d ",i);
				for(i=queenNum/2+3;i<=queenNum-1;i+=2)printf("%d ",i);
				for(i=1;i<=queenNum/2+1;i+=2)printf("%d ",i);
			}
			else
			{
				for(i=queenNum/2;i<=queenNum-1;i+=2)printf("%d ",i);
				for(i=1;i<=queenNum/2-2;i+=2)printf("%d ",i);
				for(i=queenNum/2+3;i<=queenNum;i+=2)printf("%d ",i);
				for(i=2;i<=queenNum/2+1;i+=2)printf("%d ",i);
			}
			if(queenNum%2!=0)printf("%d ",queenNum);
		}
		printf("\n");
	}
}