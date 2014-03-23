//#include<iostream>
#include<cstdio>

//using namespace std;

void main()
{
	short iCaseNum,i;
	_int64 iN,iM;
	scanf("%hd",&iCaseNum);
	for(i=1;i<=iCaseNum;i++)
	{
		scanf("%I64d%I64d",&iN,&iM);
		printf("Scenario #%hd:\n%I64d\n\n",i,(iM*iM-iN*iN+iM+iN)/2);
	}
}