#include<iostream.h>
#include<string.h>

void main()
{
	char cLeft[90],cRight[90],cTemp;
	short iCaseNum,i,j,k,iCarrier,iLeftMark,iRightMark,iSum[90];
	bool bDisplay;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>cLeft>>cRight;
		for(j=0;j<strlen(cLeft)/2;j++)
		{
			cTemp=cLeft[j];
			cLeft[j]=cLeft[strlen(cLeft)-j-1];
			cLeft[strlen(cLeft)-j-1]=cTemp;
		}
		for(j=0;j<strlen(cRight)/2;j++)
		{
			cTemp=cRight[j];
			cRight[j]=cRight[strlen(cRight)-j-1];
			cRight[strlen(cRight)-j-1]=cTemp;
		}
		//cout<<cLeft<<" "<<cRight<<endl;
		iLeftMark=iRightMark=1;
		iCarrier=0;
		for(j=0;j<strlen(cLeft) || j<strlen(cRight);j++)
		{
			if(iLeftMark && cLeft[j]=='\0')
				iLeftMark=0;
			if(iRightMark && cRight[j]=='\0')
				iRightMark=0;
			iSum[j]=((cLeft[j]-'0')*iLeftMark+(cRight[j]-'0')*iRightMark+iCarrier)%2;
			//cout<<"iSum["<<j<<"]="<<iSum[j]<<endl;
			iCarrier=((cLeft[j]-'0')*iLeftMark+(cRight[j]-'0')*iRightMark+iCarrier)/2;
			//cout<<"iCarrier="<<iCarrier<<endl;
		}
		if(iCarrier)
		{
			//cout<<"iSum["<<j<<"]="<<iCarrier<<endl;
			iSum[j++]=iCarrier;
		}
		bDisplay=false;
		cout<<i<<" ";
		for(k=j-1;k>=0;k--)
		{
			if(!bDisplay &&(k==0 || iSum[k]))
				bDisplay=true;
			if(bDisplay)
				cout<<iSum[k];
		}
		cout<<endl;
	}
}
