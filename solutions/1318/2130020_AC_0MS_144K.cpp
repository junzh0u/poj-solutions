#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
using namespace std;
typedef vector<string> VS;
typedef map<string,VS> MSVS;

void main()
{
	string strWord,strKey;int i;MSVS msvsDictionary;
	while(cin>>strWord)
	{
		if(strWord.compare("XXXXXX")==0)break;
		strKey=strWord;sort(strKey.begin(),strKey.end());
		if(msvsDictionary.find(strKey)==msvsDictionary.end())
		{
			VS vsList;vsList.push_back(strWord);
			msvsDictionary.insert(MSVS::value_type(strKey,vsList));
		}
		else msvsDictionary[strKey].push_back(strWord);
	}
	while(cin>>strKey)
	{
		if(strKey.compare("XXXXXX")==0)break;
		sort(strKey.begin(),strKey.end());
		if(msvsDictionary.find(strKey)==msvsDictionary.end())cout<<"NOT A VALID WORD"<<endl;
		else
		{
			sort(msvsDictionary[strKey].begin(),msvsDictionary[strKey].end());
			for(i=0;i<msvsDictionary[strKey].size();i++)
				cout<<msvsDictionary[strKey][i]<<endl;
		}
		cout<<"******"<<endl;
	}
}