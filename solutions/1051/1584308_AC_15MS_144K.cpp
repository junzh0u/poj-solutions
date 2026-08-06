#include<iostream>
//#include<ctype>
#include<string>

using namespace std;

void main()
{
	const string strMorse[30]={".-",		"-...",		"-.-.",		"-..",		".",
					"..-.",		"--.",		"....",		"..",		".---",
					"-.-",		".-..",		"--",		"-.",		"---",	
					".--.",		"--.-",		".-.",		"...",		"-",
					"..-",		"...-",		".--",		"-..-",		"-.--",
					"--..",		"..--",		"---.",		".-.-",		"----"};
	const string CHARACTER="ABCDEFGHIJKLMNOPQRSTUVWXYZ_.,?";
	const int iDigit[30]={2,4,4,3,1,	4,3,4,2,4,	3,4,2,2,3,	4,4,3,3,1,	3,4,3,4,4,	4,4,4,4,4};
	string strMessage,strEncrypted,strTempMorse,strOrigin;
	int iMessageNum,iLength[100],i,j,k,iTemp,iTempSum;
	cin>>iMessageNum;
	for(i=0;i<iMessageNum;i++)
	{
		cin>>strMessage;
		for(j=0;j<100;j++)
			iLength[j]=0;
		strEncrypted="";
		strOrigin="";
		for(j=0;j<strMessage.length();j++)
			for(k=0;k<30;k++)
				if(strMessage[j]==CHARACTER[k])
				{
					strEncrypted.append(strMorse[k]);
					iLength[j]=iDigit[k];
					break;
				}
		for(j=0;j<strMessage.length()/2;j++)
		{
			iTemp=iLength[j];
			iLength[j]=iLength[strMessage.length()-j-1];
			iLength[strMessage.length()-j-1]=iTemp;
		}
		iTempSum=0;
		for(j=0;j<strMessage.length();j++)
		{
			strTempMorse.assign(strEncrypted,iTempSum,iLength[j]);
			iTempSum+=iLength[j];
			for(k=0;k<30;k++)
				if(strTempMorse==strMorse[k])
				{
					strOrigin+=CHARACTER[k];
					break;
				}
		}
		//cout<<"strMessage.length(): "<<strMessage.length()<<endl;
		cout<<i+1<<": "<<strOrigin<<endl;
	}
}
