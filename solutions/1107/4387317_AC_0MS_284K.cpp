#include<iostream>
#include<string>
using namespace std;

int main()
{
	char cEnc[81],cAtoI[81],cJtoR[81],cStoZ[81];
	int iAtoI[80],iJtoR[80],iStoZ[80],iAtoINum,iJtoRNum,iStoZNum,iK1,iK2,iK3,i;
	cin>>iK1>>iK2>>iK3;
	while(iK1 || iK2 || iK3)
	{
		iAtoINum=0;
		iJtoRNum=0;
		iStoZNum=0;
		cin>>cEnc;
		for(i=0;cEnc[i]!='\0';i++)
		{
			if('a'<=cEnc[i] && cEnc[i]<='i')
			{
				cAtoI[iAtoINum]=cEnc[i];
				iAtoI[iAtoINum]=i;
				iAtoINum++;
			}
			else if('j'<=cEnc[i] && cEnc[i]<='r')
			{
				cJtoR[iJtoRNum]=cEnc[i];
				iJtoR[iJtoRNum]=i;
				iJtoRNum++;
			}
			else
			{
				cStoZ[iStoZNum]=cEnc[i];
				iStoZ[iStoZNum]=i;
				iStoZNum++;
			}
		}
		for(i=0;i<iAtoINum;i++)
			cEnc[iAtoI[(i+iK1)%iAtoINum]]=cAtoI[i];
		for(i=0;i<iJtoRNum;i++)
			cEnc[iJtoR[(i+iK2)%iJtoRNum]]=cJtoR[i];
		for(i=0;i<iStoZNum;i++)
			cEnc[iStoZ[(i+iK3)%iStoZNum]]=cStoZ[i];
		cout<<cEnc<<endl;
		cin>>iK1>>iK2>>iK3;
	}
	return 0;
}