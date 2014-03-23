#include<iostream.h>

void main()
{
	int iTable[11][11],iSide,i,j,iCase=0;
	char cChart[50][50];
	while(cin>>iSide)
	{
		if(!iSide)
			break;
		cout<<"Case "<<(++iCase)<<":\n"<<endl;
		for(i=0;i<4*iSide-3;i++)
		{
			for(j=0;j<4*iSide+1;j++)
				cChart[i][j]=' ';
			cChart[i][j]='\0';
		}
		for(i=0;i<iSide;i++)
			for(j=0;j<iSide;j++)
			{
				cin>>iTable[i][j];
				cChart[4*i][4*j+2]='O';
				if(iTable[i][j]==1)
				{
					cChart[4*i][4*j]=cChart[4*i][4*j+4]='H';
					cChart[4*i][4*j+1]=cChart[4*i][4*j+3]='-';
				}
				else if(iTable[i][j]==-1)
				{
					cChart[4*i-2][4*j+2]=cChart[4*i+2][4*j+2]='H';
					cChart[4*i-1][4*j+2]=cChart[4*i+1][4*j+2]='|';
				}
				else
				{
					if(!i || cChart[4*i-2][4*j+2]=='H')
					{
						cChart[4*i+2][4*j+2]='H';
						cChart[4*i+1][4*j+2]='|';
					}
					else
					{
						cChart[4*i-2][4*j+2]='H';
						cChart[4*i-1][4*j+2]='|';
					}
					if(cChart[4*i][4*j]=='H')
					{
						cChart[4*i][4*j+4]='H';
						cChart[4*i][4*j+3]='-';
					}
					else
					{
						cChart[4*i][4*j]='H';
						cChart[4*i][4*j+1]='-';
					}
				}
			}
		for(i=0;i<4*iSide+3;i++)
			cout<<"*";
		cout<<endl;
		for(i=0;i<4*iSide-3;i++)
			cout<<"*"<<cChart[i]<<"*"<<endl;
		for(i=0;i<4*iSide+3;i++)
			cout<<"*";
		cout<<"\n"<<endl;
	}
}