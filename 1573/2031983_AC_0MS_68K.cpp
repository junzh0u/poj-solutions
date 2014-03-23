#include<iostream.h>

void main()
{
	int iRowMax,iColMax,iRowNow,iColNow,i,iStep;
	char cMap[10][11];
	while(cin>>iRowMax>>iColMax>>iColNow)
	{
		if(iRowMax==0)
			break;
		for(i=0;i<iRowMax;i++)
			cin>>cMap[i];
		iRowNow=0;iColNow--;
		iStep=0;
		while(1)
		{
			if(iRowNow<0 || iRowNow>=iRowMax || iColNow<0 || iColNow>=iColMax)
			{
				cout<<iStep<<" step(s) to exit"<<endl;
				break;
			}
			if(cMap[iRowNow][iColNow]<=0)
			{
				cout<<(-1)*cMap[iRowNow][iColNow]<<" step(s) before a loop of "<<iStep+cMap[iRowNow][iColNow]<<" step(s)"<<endl;
				break;
			}
			switch(cMap[iRowNow][iColNow])
			{
				case 'N':{iRowNow--;cMap[iRowNow+1][iColNow]=(-1)*iStep;break;}
				case 'S':{iRowNow++;cMap[iRowNow-1][iColNow]=(-1)*iStep;break;}
				case 'W':{iColNow--;cMap[iRowNow][iColNow+1]=(-1)*iStep;break;}
				case 'E':{iColNow++;cMap[iRowNow][iColNow-1]=(-1)*iStep;break;}
			}
			iStep++;
		}
	}
}