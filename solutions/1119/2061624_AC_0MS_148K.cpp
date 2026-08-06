#include<iostream>
#include<map>
#include<string>
#include<cmath>
#include<iomanip>
#include<cctype>

using namespace std;

string NoPunctuationUpper(string strOrigin)
{
	string strTemp;
	int i;
	for(i=0;i<strOrigin.length();i++)
		if(isalpha(strOrigin[i]))
			strTemp+=tolower(strOrigin[i]);
	return strTemp;
}

void main()
{
	typedef map<string,int> msi;
	msi mDictionary,mDocument;
	msi::iterator mitPtr;
	string strSource,strSearch;
	double dScore;
	bool bEmpty;
	while(cin>>strSource)
	{
		if(strSource.compare("----------")==0)
			break;
		strSource=NoPunctuationUpper(strSource);
		if(strSource.length()==0)
			continue;
		mitPtr=mDictionary.find(strSource);
		if(mitPtr==mDictionary.end())
			mDictionary.insert(msi::value_type(strSource,1));
		else
			(mitPtr->second)++;
	}
	while(1)
	{
		bEmpty=true;
		mDocument.clear();
		while(cin>>strSearch)
		{
			if(strSearch.compare("----------")==0)
				break;
			strSearch=NoPunctuationUpper(strSearch);
			if(strSearch.length()==0)
				continue;
			bEmpty=false;
			if(mDictionary.find(strSearch)!=mDictionary.end())
			{
				mitPtr=mDocument.find(strSearch);
				if(mitPtr==mDocument.end())
					mDocument.insert(msi::value_type(strSearch,1));
				else
					(mitPtr->second)++;
			}
		}
		if(bEmpty)
			break;
		dScore=0.0;
		for(mitPtr=mDocument.begin();mitPtr!=mDocument.end();mitPtr++)
			dScore+=sqrt(mDictionary[mitPtr->first]*(mitPtr->second));
		cout<<fixed<<setprecision(2)<<dScore<<endl;
	}
}