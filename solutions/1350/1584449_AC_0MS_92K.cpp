#include<iostream.h>
#include<math.h>

const int DIGITNUM=4;

void sortUp(int[]);
void sortDown(int[]);
int calArray(int[]);

void main()
{
	int iInteger,iDigit[DIGITNUM],i,j,iMax,iMin;
	cin>>iInteger;
	while(iInteger!=-1)
	{
		cout<<"N="<<iInteger<<":"<<endl;
		if(iInteger<1000 || iInteger>9999 || iInteger%1111==0)
			cout<<"No!!"<<endl;
		else
		{
			for(i=1;;i++)
			{
				for(j=0;j<DIGITNUM;j++)
				{
					iDigit[j]=iInteger%10;
					iInteger/=10;
				}
				sortUp(iDigit);
				iMax=calArray(iDigit);
				sortDown(iDigit);
				iMin=calArray(iDigit);
				iInteger=iMax-iMin;
				cout<<iMax<<"-"<<iMin<<"="<<iInteger<<endl;
				if(iInteger==0 || iInteger==6174)
					break;
			}
			cout<<"Ok!! "<<i<<" times"<<endl;
		}
		cin>>iInteger;
	}
}

void sortUp(int iDigit[])
{
	int i,j,iTemp,iEmptyDigit=0;
	for(i=DIGITNUM-1;i>=0;i--)
	{
		if(iDigit[i]!=0)
			break;
		else
			iEmptyDigit++;
	}
	for(i=0;i<DIGITNUM-iEmptyDigit-1;i++)
		for(j=i+1;j<DIGITNUM-iEmptyDigit;j++)
			if(iDigit[i]>iDigit[j])
			{
				iTemp=iDigit[i];
				iDigit[i]=iDigit[j];
				iDigit[j]=iTemp;
			}
}

void sortDown(int iDigit[])
{
	int i,j,iTemp;
	for(i=0;i<DIGITNUM-1;i++)
		for(j=i+1;j<DIGITNUM;j++)
			if(iDigit[i]<iDigit[j])
			{
				iTemp=iDigit[i];
				iDigit[i]=iDigit[j];
				iDigit[j]=iTemp;
			}
}

int calArray(int iDigit[])
{
	int i,iSum=0;
	for(i=0;i<DIGITNUM;i++)
		iSum+=iDigit[i]*pow(10,i);
	return iSum;
}