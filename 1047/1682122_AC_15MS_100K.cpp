#include<iostream>
#include<string>

using namespace std;

string Multiply(string,int);

void main()
{
	string strIni,strStep;
	int i,j,k,iTemp;
	bool bCycle;
	while(cin>>strIni)
	{
		strStep=strIni;
		bCycle=true;
		for(i=2;i<=strIni.length() && bCycle;i++)
		{
			strStep=Multiply(strIni,i);
			if(strStep=="")
			{
				bCycle=false;
				break;
			}
			else
			{
				iTemp=-1;
				while(bCycle)
				{
					for(j=iTemp+1;j<strIni.length();j++)
						if(strStep[j]==strIni[0])
							break;
					if(j==strIni.length())
					{
						bCycle=false;
						break;
					}
					else
					{
						for(k=1;k<strIni.length();k++)
							if(strStep[(j+k)%strIni.length()]!=strIni[k])
							{
								bCycle=false;
								break;
							}
						if(bCycle)
							break;
						else
							bCycle=true;
					}
					iTemp=j;
				}
			}
		}
		cout<<strIni<<" is"<<(bCycle?" ":" not ")<<"cyclic"<<endl;
	}
}

string Multiply(string strMulCand,int iMulEr)
{
	int i,iTemp;
	string strRet="";
	for(i=0;i<strMulCand.length();i++)
		strRet.append("0");
	for(i=strMulCand.length()-1;i>=0;i--)
	{
		iTemp=strRet[i]+(strMulCand[i]-'0')*iMulEr;
		if(iTemp-'0'>=10)
		{
			if(i)
				strRet[i-1]+=(iTemp-'0')/10;
			else
				return "";
		}
		strRet[i]=(iTemp-'0')%10+'0';
	}
	return strRet;
}