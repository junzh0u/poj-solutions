#include<iostream.h>
#include<string.h>

void main()
{
	int iNum,iDivBase,iIn,i;
	bool bRemain[100]={true},bRemTemp[100];
	cin>>iNum>>iDivBase;
	while(iNum--)
	{
		cin>>iIn;
		memset(bRemTemp,false,iDivBase*sizeof(bool));
		for(i=0;i<iDivBase;i++)
			if(bRemain[i])
			{
				bRemTemp[((i+iIn)%iDivBase+iDivBase)%iDivBase]=true;
				bRemTemp[((i-iIn)%iDivBase+iDivBase)%iDivBase]=true;
			}
		memcpy(bRemain,bRemTemp,sizeof(bRemain));
	}
	cout<<(bRemain[0]?"Divisible":"Not divisible")<<endl;
}