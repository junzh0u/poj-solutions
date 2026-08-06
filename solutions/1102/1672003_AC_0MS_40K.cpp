#include<iostream.h>

void main()
{
	const char cHead[11]="- -- -----",cFoot[11]="- -- -- --",cMid[11]="  ----- --",
		cHeadMidLeft[11]="|   ||| ||",cHeadMidRight[11]="|||||  |||",
		cMidFootLeft[11]="| |   | | ",cMidFootRight[11]="|| |||||||";
	int iS,i,j,k;
	char cN[9];
	while(1)
	{
		cin>>iS;
		cin>>cN;
		if(iS==0)
			break;
		for(i=0;cN[i]!='\0';i++)
		{
			cout<<" ";
			for(j=0;j<iS;j++)
				cout<<cHead[cN[i]-'0'];
			cout<<"  ";
		}
		cout<<endl;
		for(k=0;k<iS;k++)
		{
			for(i=0;cN[i]!='\0';i++)
			{
				cout<<cHeadMidLeft[cN[i]-'0'];
				for(j=0;j<iS;j++)
					cout<<" ";
				cout<<cHeadMidRight[cN[i]-'0'];
				cout<<" ";
			}
			cout<<endl;
		}
		for(i=0;cN[i]!='\0';i++)
		{
			cout<<" ";
			for(j=0;j<iS;j++)
				cout<<cMid[cN[i]-'0'];
			cout<<"  ";
		}
		cout<<endl;
		for(k=0;k<iS;k++)
		{
			for(i=0;cN[i]!='\0';i++)
			{
				cout<<cMidFootLeft[cN[i]-'0'];
				for(j=0;j<iS;j++)
					cout<<" ";
				cout<<cMidFootRight[cN[i]-'0'];
				cout<<" ";
			}
			cout<<endl;
		}
		for(i=0;cN[i]!='\0';i++)
		{
			cout<<" ";
			for(j=0;j<iS;j++)
				cout<<cFoot[cN[i]-'0'];
			cout<<"  ";
		}
		cout<<'\n'<<endl;
	}
}