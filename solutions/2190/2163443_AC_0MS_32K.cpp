#include<iostream.h>

void main()
{
	char cNumber[11];int iPos,i,iSum=0;
	cin>>cNumber;
	for(iPos=0;cNumber[iPos]!='?';iPos++);
	for(i=0;i<10;i++)
	{
		if(i==iPos)continue;
		if(cNumber[i]=='X')
		{
			//cout<<(10-i)*10<<endl;
			iSum+=(10-i)*10;
		}
		else
		{
			//cout<<(10-i)*(cNumber[i]-'0')<<endl;
			iSum+=(10-i)*(cNumber[i]-'0');
		}
	}
	//cout<<iSum<<endl;
	for(i=0;i<=10;i++)
		if((iSum+(10-iPos)*i)%11==0)
			break;
	if(i>10)cout<<"-1"<<endl;
	else if(i<10)cout<<i<<endl;
	else if(iPos==9)cout<<"X"<<endl;
	else cout<<"-1"<<endl;
}