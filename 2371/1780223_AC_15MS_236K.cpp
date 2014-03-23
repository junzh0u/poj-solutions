#include<algorithm>
#include<stdio.h>

void main()
{
	int iBaseNum,iQuery,i;
	short iDataBase[100000],iQueryNum;
	scanf("%d",&iBaseNum);
	for(i=0;i<iBaseNum;i++)
		scanf("%d",&iDataBase[i]);
	std::sort(iDataBase,iDataBase+iBaseNum);
	getchar();getchar();getchar();getchar();
	scanf("%d",&iQueryNum);
	while(iQueryNum--)
	{
		scanf("%d",&iQuery);
		printf("%d\n",iDataBase[iQuery-1]);
	}
}