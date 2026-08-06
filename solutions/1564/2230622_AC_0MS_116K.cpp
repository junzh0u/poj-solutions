#include<iostream>
#include<map>
//#include<algorithm>
using namespace std;

typedef map<int,int,greater<int> > MII;
typedef MII::iterator MIT;

MII miiList;
MIT mitList[12];
int iUse[12],iCount;

void Try(int iCur,int iRemain)
{
	int i,j,iNum,iValue;bool bFirst;
	if(iCur==miiList.size())
	{
		if(iRemain==0)
		{
			bFirst=true;
			for(i=0;i<iCur;i++)
				for(j=0;j<iUse[i];j++)
				{
					if(!bFirst)cout<<"+";
					cout<<mitList[i]->first;
					bFirst=false;
				}
			cout<<endl;
			iCount++;
		}
		return;
	}
	iNum=mitList[iCur]->second;iValue=mitList[iCur]->first;
	for(iUse[iCur]=iNum;iUse[iCur]>=0;iUse[iCur]--)
		Try(iCur+1,iRemain-iUse[iCur]*iValue);
}

void main()
{
	int iTotal,iNum,iTemp,i;
	MIT mitTemp;
	while(cin>>iTotal>>iNum && iNum>0)
	{
		miiList.clear();iCount=0;
		for(i=0;i<iNum;i++)
		{
			cin>>iTemp;
			if(miiList.find(iTemp)==miiList.end())
				miiList.insert(MII::value_type(iTemp,1));
			else
				miiList[iTemp]++;
		}
		for(mitTemp=miiList.begin(),i=0;i<=miiList.size();mitTemp++,i++)
			mitList[i]=mitTemp;
		cout<<"Sums of "<<iTotal<<":"<<endl;
		Try(0,iTotal);
		if(iCount==0)cout<<"NONE"<<endl;
	}
}