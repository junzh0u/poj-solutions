#include<iostream.h>

int Round(int,int);

void main()
{
	int iCaseNum,iInput;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iInput;
		cout<<Round(iInput,10)<<endl;
	}
}

int Round(int iInput,int iBase)
{
	if(iInput>iBase)
	{
		if((iInput%iBase)/(iBase/10)>=5)
			iInput=((iInput/iBase)+1)*iBase;
		else
			iInput=(iInput/iBase)*iBase;
		return Round(iInput,iBase*10);
	}
	else
		return iInput;
}