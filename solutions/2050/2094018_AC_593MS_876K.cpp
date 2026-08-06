#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<cctype>
using namespace std;

#define MAX1 110
#define MAX2 2000

typedef vector<int> VI;
typedef map<string,VI> MSVI;

MSVI mInvertIndex;
string strDocument[MAX2];
int iDocumentNum,iDocumentIndex[MAX1]={0},iLineNum=0,iLineToDocument[MAX2],i,j;

VI SearchTerm(string strTerm)
{
	VI vEmpty;
	if(mInvertIndex.find(strTerm)!=mInvertIndex.end())
		return mInvertIndex[strTerm];
	else
		return vEmpty;
}

VI NotLogic(VI vOrigin)
{
	VI vDeny;
	bool bExist[MAX1]={false};
	for(i=0;i<vOrigin.size();i++)
		bExist[iLineToDocument[vOrigin[i]]]=true;
	vDeny.push_back(-1);
	for(i=0;i<iDocumentNum;i++)
		if(!bExist[i])
			vDeny.push_back(i);
	return vDeny;
}

VI OrLogic(VI vLeft,VI vRight)
{
	VI vUnite;
	int iLeftPtr=0,iRightPtr=0;
	while(iLeftPtr<vLeft.size() && iRightPtr<vRight.size())
	{
		if(vLeft[iLeftPtr]==vRight[iRightPtr])
		{
			vUnite.push_back(vLeft[iLeftPtr++]);
			iRightPtr++;
		}
		else if(vLeft[iLeftPtr]<vRight[iRightPtr])
			vUnite.push_back(vLeft[iLeftPtr++]);
		else
			vUnite.push_back(vRight[iRightPtr++]);
	}
	while(iLeftPtr<vLeft.size())
		vUnite.push_back(vLeft[iLeftPtr++]);
	while(iRightPtr<vRight.size())
		vUnite.push_back(vRight[iRightPtr++]);
	return vUnite;
}

VI AndLogic(VI vLeft,VI vRight)
{
	VI vUnite,vLeftTemp,vRightTemp;
	bool bLeftExist[MAX1]={false},bRightExist[MAX1]={false},bExist[MAX1]={false};
	int iLeftPtr=0,iRightPtr=0;
	for(i=0;i<vLeft.size();i++)
		bLeftExist[iLineToDocument[vLeft[i]]]=true;
	for(i=0;i<vRight.size();i++)
		bRightExist[iLineToDocument[vRight[i]]]=true;
	for(i=0;i<vLeft.size();i++)
		if(bRightExist[iLineToDocument[vLeft[i]]])
			vLeftTemp.push_back(vLeft[i]);
	for(i=0;i<vRight.size();i++)
		if(bLeftExist[iLineToDocument[vRight[i]]])
			vRightTemp.push_back(vRight[i]);
	return OrLogic(vLeftTemp,vRightTemp);
}

void ExpandIndex(string strLine,int iLine)
{
	int iFirst=0,iLast;
	string strTerm;
	for(;iFirst<strLine.length();iFirst++)
	{
		if(isalpha(strLine[iFirst]))
		{
			for(iLast=iFirst;iLast<strLine.length() && isalpha(strLine[iLast]);iLast++);
			strTerm.assign(strLine,iFirst,iLast-iFirst);
			for(i=0;i<strTerm.length();i++)
				strTerm[i]=tolower(strTerm[i]);
			if(mInvertIndex.find(strTerm)==mInvertIndex.end())
			{
				VI vBucket;
				vBucket.push_back(iLine);
				mInvertIndex.insert(MSVI::value_type(strTerm,vBucket));
			}
			else
				if(mInvertIndex[strTerm][mInvertIndex[strTerm].size()-1]!=iLine)
					mInvertIndex[strTerm].push_back(iLine);
			iFirst=iLast;
		}
	}
}

void main()
{
	int iCurrentDocument=0;
	int iQueryNum,iSpacePosition[2];
	string strQuery,strLeft,strRight;
	char cTemp[100];
	VI vResult;
	cin>>iDocumentNum;cin.ignore();
	while(iCurrentDocument<iDocumentNum && getline(cin,strDocument[iLineNum++]))
	{
		if(strDocument[iLineNum-1].compare("**********")==0)
		{
			iDocumentIndex[++iCurrentDocument]=iLineNum;
			for(i=iDocumentIndex[iCurrentDocument-1];i<iDocumentIndex[iCurrentDocument];i++)
				iLineToDocument[i]=iCurrentDocument-1;
		}
		else
			ExpandIndex(strDocument[iLineNum-1],iLineNum-1);
	}
	cin>>iQueryNum;cin.ignore();
	while(iQueryNum--)
	{
		getline(cin,strQuery);
		vResult.erase(vResult.begin(),vResult.end());
		iSpacePosition[0]=strQuery.find(" ");
		if(iSpacePosition[0]==-1)
			vResult=SearchTerm(strQuery);
		else
		{
			iSpacePosition[1]=strQuery.find(" ",iSpacePosition[0]+1);
			if(iSpacePosition[1]==-1)
			{
				strLeft.assign(strQuery,4,strQuery.length());
				vResult=NotLogic(SearchTerm(strLeft));
			}
			else
			{
				strLeft.assign(strQuery,0,iSpacePosition[0]);
				strRight.assign(strQuery,iSpacePosition[1]+1,strQuery.length());
				if(strQuery[iSpacePosition[0]+1]=='A')
					vResult=AndLogic(SearchTerm(strLeft),SearchTerm(strRight));
				else
					vResult=OrLogic(SearchTerm(strLeft),SearchTerm(strRight));
			}
		}
		if(vResult.size()==0)
			cout<<"Sorry, I found nothing."<<endl;
		else
		{
			if(vResult[0]!=-1)
			{
				iCurrentDocument=iLineToDocument[vResult[0]];
				for(i=0;i<vResult.size();i++)
				{
					if(iLineToDocument[vResult[i]]!=iCurrentDocument)
					{
						cout<<"----------"<<endl;
						iCurrentDocument=iLineToDocument[vResult[i]];
					}
					cout<<strDocument[vResult[i]]<<endl;
				}
			}
			else
			{
				for(i=1;i<vResult.size();i++)
				{
					for(j=iDocumentIndex[vResult[i]];j<iDocumentIndex[vResult[i]+1]-1;j++)
						cout<<strDocument[j]<<endl;
					if(i!=vResult.size()-1)
						cout<<"----------"<<endl;
				}
			}
		}
		cout<<"=========="<<endl;
	}
}