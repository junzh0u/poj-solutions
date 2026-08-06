#include<iostream.h>
#include<stdlib.h>
#include<iomanip.h>

int compare(const void * a,const void * b)
	{ return *((int*)a)-*((int*)b); }

int main()
{
	int iCaseNum,i,j,k,iCount,iNonTru=1;
	char cTemp[100],cCode[8],cEncode[]={"22233344455566677778889999"};
	cin>>iCaseNum;
	int *iCode=new int [iCaseNum];
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cTemp;
		k=0;
		for(j=0;cTemp[j];j++)
		{
			if(cTemp[j]>='A' && cTemp[j]<='Z')
			{
				cCode[k]=cEncode[cTemp[j]-'A'];
				k++;
			}
			else if(cTemp[j]>='0' && cTemp[j]<='9')
			{
				cCode[k]=cTemp[j];
				k++;
			}
		}
		iCode[i]=atoi(cCode);
	}
	qsort(iCode,iCaseNum,sizeof(int),compare);
	for(i=0;i<iCaseNum;)
	{
		iCount=1;
		for(j=i+1;j<iCaseNum;j++)
		{
			if(iCode[i]==iCode[j])
				iCount++;
			else
				break;
		}
		if(iCount>1)
		{
			iNonTru=0;
			cout<<setfill('0')<<setw(3)<<iCode[i]/10000<<"-"<<setfill('0')<<setw(4)<<iCode[i]%10000<<" "<<iCount<<endl;
		}
		i=j;
	}
	if(iNonTru)
		cout<<"No duplicates."<<endl;
	delete [] iCode;
	return 0;
}