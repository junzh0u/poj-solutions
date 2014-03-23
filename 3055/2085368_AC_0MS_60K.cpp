#include<iostream.h>
#include<string.h>

bool IsFriend(char* cLeft,char* cRight)
{
	bool bNumberExist[2][10]={false};
	int i;
	for(i=0;cLeft[i]!='\0';i++)
		bNumberExist[0][cLeft[i]-'0']=true;
	for(i=0;cRight[i]!='\0';i++)
		bNumberExist[1][cRight[i]-'0']=true;
	for(i=0;i<10;i++)
		if(bNumberExist[0][i]!=bNumberExist[1][i])
			return false;
	return true;
}

bool NeighbourExchangeLeftDown(char* cString,int iLeft)
{
	if(cString[iLeft+1]=='\0' || cString[iLeft]=='0' || cString[iLeft+1]=='9' || (iLeft==0 && cString[iLeft]=='1'))
		return false;
	cString[iLeft]--;cString[iLeft+1]++;
	return true;
}

bool NeighbourExchangeLeftUp(char* cString,int iLeft)
{
	if(cString[iLeft+1]=='\0' || cString[iLeft]=='9' || cString[iLeft+1]=='0')
		return false;
	cString[iLeft]++;cString[iLeft+1]--;
	return true;
}

bool IsAlmostFriend(char* cLeft,char* cRight)
{
	char cTemp[200];
	int i;
	for(i=0;i<strlen(cLeft)-1;i++)
	{
		strcpy(cTemp,cLeft);
		if(NeighbourExchangeLeftDown(cTemp,i) && IsFriend(cTemp,cRight))
			return true;
		strcpy(cTemp,cLeft);
		if(NeighbourExchangeLeftUp(cTemp,i) && IsFriend(cTemp,cRight))
			return true;
	}
	return false;
}

void main()
{
	int iCaseNum;
	char cLeft[200],cRight[200];
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>cLeft>>cRight;
		if(IsFriend(cLeft,cRight))
			cout<<"friends"<<endl;
		else if(IsAlmostFriend(cLeft,cRight) || IsAlmostFriend(cRight,cLeft))
			cout<<"almost friends"<<endl;
		else
			cout<<"nothing"<<endl;
	}
}