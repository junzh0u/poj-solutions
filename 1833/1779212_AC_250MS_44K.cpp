#include<algorithm>
#include<cstdio>

void main()
{
	int iCaseNum,iLength,iStep,iArray[1024],i;
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d%d",&iLength,&iStep);
		for(i=0;i<iLength;i++)
			scanf("%d",&iArray[i]);
		for(i=0;i<iStep;i++)
			std::next_permutation(iArray,iArray+iLength);
		for(i=0;i<iLength;i++)
			printf("%d ",iArray[i]);
		printf("\n");
	}
}