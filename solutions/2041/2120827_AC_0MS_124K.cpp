#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

int NameToCode(char cName)
{
	switch(cName)
	{
		case 'J':return 0;
		case 'C':return 1;
		case 'E':return 2;
		case 'A':return 3;
		case 'P':return 4;
		case 'M':return 5;
		default:return -1;
	}
}

void MrJ(string* psMessage)
{
	char cEnd=(*psMessage)[psMessage->length()-1];int i;
	for(i=psMessage->length()-1;i>=1;i--)(*psMessage)[i]=(*psMessage)[i-1];
	(*psMessage)[0]=cEnd;
}
void MissC(string* psMessage)
{
	char cBegin=(*psMessage)[0];int i;
	for(i=0;i<psMessage->length()-1;i++)(*psMessage)[i]=(*psMessage)[i+1];
	(*psMessage)[psMessage->length()-1]=cBegin;
}
void MrE(string* psMessage)
{
	char cTemp;int i;
	for(i=0;i<psMessage->length()/2;i++)
	{
		cTemp=(*psMessage)[i];
		(*psMessage)[i]=(*psMessage)[i+psMessage->length()-psMessage->length()/2];
		(*psMessage)[i+psMessage->length()-psMessage->length()/2]=cTemp;
	}
}
void MrA(string* psMessage){reverse(psMessage->begin(),psMessage->end());}
void DrP(string* psMessage)
{
	for(int i=0;i<psMessage->length();i++)
		if(isdigit((*psMessage)[i]))
			(*psMessage)[i]=((*psMessage)[i]-'0'+9)%10+'0';
}
void MrM(string* psMessage)
{
	for(int i=0;i<psMessage->length();i++)
		if(isdigit((*psMessage)[i]))
			(*psMessage)[i]=((*psMessage)[i]-'0'+1)%10+'0';
}
void(*Servant[6])(string* psMessage)={MrJ,MissC,MrE,MrA,DrP,MrM};

void main()
{
	int iCaseNum,i;
	string strOrder,strMessage;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>strOrder>>strMessage;
		for(i=strOrder.length()-1;i>=0;i--)
			(*Servant[NameToCode(strOrder[i])])(&strMessage);
		cout<<strMessage<<endl;
	}
}