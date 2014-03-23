#include<stdio.h>
#include<math.h>

void main()
{
	unsigned long iCaseNum,iRoot,iLevel,i,iTemp;
	scanf("%ld",&iCaseNum);
	for(i=0;i<iCaseNum;i++)
	{
		scanf("%ld",&iRoot);
		iTemp=iRoot;
		iLevel=0;
		while(!(iTemp%2))
		{
			iLevel++;
			iTemp/=2;
		}
		printf("%ld %ld\n",(unsigned long)(iRoot-pow(2,iLevel)+1),(unsigned long)(iRoot+pow(2,iLevel)-1));
	}
}
