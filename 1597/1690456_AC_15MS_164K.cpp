#include<iostream.h>
#include<iomanip.h>

void main()
{
	int iStep,iMod,i,iSeed;
	bool bOccur[100000];
	while(cin>>iStep>>iMod)
	{
		for(i=0;i<iMod;i++)
			bOccur[i]=false;
		iSeed=0;
		while(1)
		{
			if(!bOccur[iSeed])
				bOccur[iSeed]=true;
			else
				break;
			iSeed=(iSeed+iStep)%iMod;
		}
		for(i=0;i<iMod;i++)
			if(bOccur[i]==false)
			{
				cout<<setw(10)<<iStep<<setw(10)<<iMod<<"    Bad Choice\n"<<endl;
				break;
			}
		if(i==iMod)
			cout<<setw(10)<<iStep<<setw(10)<<iMod<<"    Good Choice\n"<<endl;
	}
}