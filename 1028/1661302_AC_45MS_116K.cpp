#include<iostream>
#include<string>

using namespace std;

struct URL
{
	string strName;
	URL *pPre;
	URL *pNext;
};

void main()
{
	URL uHome,*uPtrNow,*uPtrTemp;
	uHome.strName="http://www.acm.org/";
	uHome.pPre=NULL;
	uHome.pNext=NULL;
	uPtrNow=&uHome;
	string strCom,strUrl;
	cin>>strCom;
	while(strCom!="QUIT")
	{
		if(strCom=="VISIT")
		{
			strUrl="";
			cin>>strUrl;
			//delete uPtrNow->pNext;
			uPtrNow->pNext=new URL;
			uPtrTemp=uPtrNow;
			uPtrNow=uPtrNow->pNext;
			uPtrNow->pPre=uPtrTemp;
			uPtrNow->pNext=NULL;
			uPtrNow->strName=strUrl;
			cout<<uPtrNow->strName<<endl;
		}
		else if(strCom=="BACK")
		{
			if(uPtrNow->pPre==NULL)
				cout<<"Ignored"<<endl;
			else
			{
				uPtrNow=uPtrNow->pPre;
				cout<<uPtrNow->strName<<endl;
			}
		}
		else if(strCom=="FORWARD")
		{
			if(uPtrNow->pNext==NULL)
				cout<<"Ignored"<<endl;
			else
			{
				uPtrNow=uPtrNow->pNext;
				cout<<uPtrNow->strName<<endl;
			}
		}
		strCom="";
		cin>>strCom;
	}
}