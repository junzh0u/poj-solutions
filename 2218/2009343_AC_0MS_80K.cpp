#include<cstdio>
#include<map>
#include<string>

using namespace std;

void main()
{
	typedef map<int,string,greater<int> > mic;
	mic mList;
	mic::const_iterator mtPtr;
	char cTemp[40],cName[30];
	int iDay,iWeight;
	while(gets(cTemp))
	{
		if(!strcmp(cTemp,"START"))
			mList.clear();
		else if(!strcmp(cTemp,"END"))
		{
			for(mtPtr=mList.begin();mtPtr!=mList.end();mtPtr++)
				printf("%s\n",mtPtr->second.c_str());
			printf("\n");
		}
		else
		{
			sscanf(cTemp,"%s %d %d",cName,&iDay,&iWeight);
			mList.insert(mic::value_type(iWeight-iDay,cName));
		}
	}
}