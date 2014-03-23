#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
#define MAX 100

struct Directory
{int iSubNum,iFileNum;Directory* dSub[MAX];string strName,strFile[MAX];};

void Clear(Directory& dRoot){dRoot.iSubNum=dRoot.iFileNum=0;}

bool Input(Directory& dRoot)
{
	string strLine;
	Clear(dRoot);
	while(cin>>strLine && strLine[0]!=']' && strLine[0]!='*' && strLine[0]!='#')
	{
		switch(strLine[0])
		{
		case'f':dRoot.strFile[dRoot.iFileNum++]=strLine;break;
		case'd':
			dRoot.dSub[dRoot.iSubNum]=new Directory;
			dRoot.dSub[dRoot.iSubNum]->strName=strLine;
			Input(*dRoot.dSub[dRoot.iSubNum]);
			dRoot.iSubNum++;
			break;
		default:break;
		}
	}
	sort(dRoot.strFile,dRoot.strFile+dRoot.iFileNum);
	return strLine[0]!='#';
}

void Print(Directory dRoot,string strPre)
{
	int i;
	cout<<strPre<<dRoot.strName<<endl;
	for(i=0;i<dRoot.iSubNum;i++)Print(*dRoot.dSub[i],strPre+"|     ");
	for(i=0;i<dRoot.iFileNum;i++)cout<<strPre<<dRoot.strFile[i]<<endl;
}

int main()
{
	Directory dRoot;dRoot.strName="ROOT";int iCount=1;
	while(Input(dRoot))
	{cout<<"DATA SET "<<iCount++<<":"<<endl;Print(dRoot,"");cout<<endl;}
	return 0;
}