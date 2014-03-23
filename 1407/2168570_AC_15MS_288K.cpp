#include<iostream>
#include<string>
#include<map>
#include<vector>
using namespace std;

typedef pair<int,int> PII;
typedef map<string,PII> MSPII;
typedef vector<int> VI;

struct Order{string strDealer,strType;char cCommodity;int iPrice;bool bDone;} oRecord[1000];
struct Commodity{VI viPrice;} cList[26];
MSPII mspiiDealer;

void main()
{
	int iOrderNum,i,j,iChoose,iMax,iMin,iAverage;
	MSPII::const_iterator mspiiIter;
	while(cin>>iOrderNum)
	{
		if(iOrderNum==0)break;
		mspiiDealer.clear();
		for(i=0;i<26;i++)cList[i].viPrice.clear();
		for(i=0;i<iOrderNum;i++)
		{
			cin>>oRecord[i].strDealer>>oRecord[i].strType>>oRecord[i].cCommodity>>oRecord[i].iPrice;
			oRecord[i].bDone=false;iChoose=-1;
			if(mspiiDealer.find(oRecord[i].strDealer)==mspiiDealer.end())
			{
				PII piiTemp(0,0);
				mspiiDealer.insert(MSPII::value_type(oRecord[i].strDealer,piiTemp));
			}
			for(j=0;j<i;j++)
				if(!oRecord[j].bDone && oRecord[j].cCommodity==oRecord[i].cCommodity && oRecord[j].strType.compare(oRecord[i].strType)!=0 && oRecord[j].strDealer.compare(oRecord[i].strDealer)!=0)
				{
					if(iChoose==-1)
					{
						if(oRecord[i].strType.compare("SELL")==0 && oRecord[j].iPrice>=oRecord[i].iPrice)
							iChoose=j;
						else if(oRecord[i].strType.compare("BUY")==0 && oRecord[j].iPrice<=oRecord[i].iPrice)
							iChoose=j;
					}
					else if(oRecord[i].strType.compare("SELL")==0 && oRecord[j].iPrice>oRecord[iChoose].iPrice)
						iChoose=j;
					else if(oRecord[i].strType.compare("BUY")==0 && oRecord[j].iPrice<oRecord[iChoose].iPrice)
						iChoose=j;
				}
			if(iChoose!=-1)
			{
				oRecord[i].bDone=oRecord[iChoose].bDone=true;
				cList[oRecord[i].cCommodity-'A'].viPrice.push_back((oRecord[i].iPrice+oRecord[iChoose].iPrice)/2);
				if(oRecord[i].strType.compare("BUY")==0)
					mspiiDealer[oRecord[i].strDealer].first+=(oRecord[i].iPrice+oRecord[iChoose].iPrice)/2;
				else
					mspiiDealer[oRecord[i].strDealer].second+=(oRecord[i].iPrice+oRecord[iChoose].iPrice)/2;
				if(oRecord[iChoose].strType.compare("BUY")==0)
					mspiiDealer[oRecord[iChoose].strDealer].first+=(oRecord[i].iPrice+oRecord[iChoose].iPrice)/2;
				else
					mspiiDealer[oRecord[iChoose].strDealer].second+=(oRecord[i].iPrice+oRecord[iChoose].iPrice)/2;
			}
		}
		for(i=0;i<26;i++)
			if(!cList[i].viPrice.empty())
			{
				cout<<char('A'+i)<<" ";
				iAverage=iMax=iMin=cList[i].viPrice[0];
				for(j=1;j<cList[i].viPrice.size();j++)
				{
					iAverage+=cList[i].viPrice[j];
					if(cList[i].viPrice[j]>iMax)
						iMax=cList[i].viPrice[j];
					if(cList[i].viPrice[j]<iMin)
						iMin=cList[i].viPrice[j];
				}
				cout<<iMin<<" "<<int(iAverage/cList[i].viPrice.size())<<" "<<iMax<<endl;
			}
		cout<<"--"<<endl;
		for(mspiiIter=mspiiDealer.begin();mspiiIter!=mspiiDealer.end();mspiiIter++)
			cout<<mspiiIter->first<<" "<<mspiiIter->second.first<<" "<<mspiiIter->second.second<<endl;
		cout<<"----------"<<endl;
	}
}