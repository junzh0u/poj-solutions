#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
#define MAX 100

struct Relation{string strFather,strChild;int iDiff;} rList[MAX];
struct Man{string strName;int iAge;} mDescendant[2*MAX];
int iRelationNum,iDescendantNum;

bool ManCmp(Man mLeft,Man mRight)
{
	if(mLeft.iAge!=mRight.iAge)
		return mLeft.iAge>mRight.iAge;
	return mLeft.strName<mRight.strName;
}

void Calculate(string strFather,int iAge)
{
	int i;
	for(i=0;i<iRelationNum;i++)
		if(rList[i].strFather.compare(strFather)==0)
		{
			mDescendant[iDescendantNum].strName=rList[i].strChild;
			mDescendant[iDescendantNum++].iAge=iAge-rList[i].iDiff;
			Calculate(rList[i].strChild,iAge-rList[i].iDiff);
		}
}

void main()
{
	int iCaseNum,iCase,i;
	cin>>iCaseNum;
	for(iCase=1;iCase<=iCaseNum;iCase++)
	{
		cout<<"DATASET "<<iCase<<endl;
		cin>>iRelationNum;
		for(i=0;i<iRelationNum;i++)
			cin>>rList[i].strFather>>rList[i].strChild>>rList[i].iDiff;
		iDescendantNum=0;
		Calculate("Ted",100);
		sort(mDescendant,&mDescendant[iDescendantNum],ManCmp);
		for(i=0;i<iDescendantNum;i++)
			cout<<mDescendant[i].strName<<" "<<mDescendant[i].iAge<<endl;
	}
}