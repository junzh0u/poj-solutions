#include<iostream>
#include<map>
#include<string>
using namespace std;

typedef map<string,int> MSI;
MSI msiDictionary;
MSI::const_iterator msiIter;

typedef multimap<int,string,greater<int> > MMIS;
MMIS mmisFrequency;
MMIS::const_iterator mmisIter;

void main()
{
	string strText,strLine,strDigram;int iLineNum,i;
	while(cin>>iLineNum && iLineNum!=0)
	{
		cin.ignore();
		strText="";
		while(iLineNum--)
		{
			getline(cin,strLine);
			strText.append(strLine);
		}
		msiDictionary.clear();
		for(i=0;i<strText.length()-1;i++)
		{
			strDigram=strText[i];strDigram+=strText[i+1];
			if(msiDictionary.find(strDigram)==msiDictionary.end())
				msiDictionary.insert(MSI::value_type(strDigram,1));
			else msiDictionary[strDigram]++;
		}
		mmisFrequency.clear();
		for(msiIter=msiDictionary.begin();msiIter!=msiDictionary.end();msiIter++)
			mmisFrequency.insert(MMIS::value_type(msiIter->second,msiIter->first));
		cout.setf(ios::fixed);cout.precision(6);
		for(mmisIter=mmisFrequency.begin(),i=0;mmisIter!=mmisFrequency.end(),i<5;mmisIter++,i++)
			cout<<mmisIter->second<<" "<<mmisIter->first<<" "<<mmisIter->first/double(strText.length()-1)<<endl;
		cout<<endl;
	}
}