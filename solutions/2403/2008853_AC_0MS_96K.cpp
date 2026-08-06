#include<cstdio>
#include<map>
#include<string>
using namespace std;

void main()
{
	typedef map<string,int> mci;
	mci mDict;
	int iWordNum,iDescNum,iValue,iSum;
	char cWord[20];
	scanf("%d %d",&iWordNum,&iDescNum);
	while(iWordNum--)
	{
		scanf("%s %d",cWord,&iValue);
		mDict.insert(mci::value_type(cWord,iValue));
	}
	while(iDescNum--)
	{
		iSum=0;
		while(scanf("%s",cWord))
		{
			if(cWord[0]=='.')
				break;
			iSum+=mDict[cWord];
		}
		printf("%d\n",iSum);
	}
}