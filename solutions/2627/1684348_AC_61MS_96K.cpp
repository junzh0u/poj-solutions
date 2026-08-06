#include<iostream.h>

void main()
{
	double dVel,dMin,dHole[1100][3],dDisSqu;
	int iCount=0,i,j,k;
	bool bCanMake=true;
	cin>>dVel>>dMin;
	dDisSqu=dVel*dMin*60*dVel*dMin*60;
	while(cin>>dHole[iCount][0]>>dHole[iCount][1])
	{
		dHole[iCount][2]=-1;
		iCount++;
	}
	dHole[1][2]=0;
	for(i=0;bCanMake;i++)
	{
		bCanMake=false;
		for(j=0;j<iCount;j++)
			if(dHole[j][2]==i)
			{
				for(k=0;k<iCount;k++)
				{
					if(dHole[k][2]!=-1)
						continue;
					if((dHole[j][0]-dHole[k][0])*(dHole[j][0]-dHole[k][0])+(dHole[j][1]-dHole[k][1])*(dHole[j][1]-dHole[k][1])<=dDisSqu)
					{
						dHole[k][2]=i+1;
						bCanMake=true;
					}
				}
			}
		if(dHole[0][2]!=-1)
			break;
	}
	if(dHole[0][2]!=-1)
		cout<<"Yes, visiting "<<dHole[0][2]-1<<" other holes."<<endl;
	else
		cout<<"No." <<endl;
}