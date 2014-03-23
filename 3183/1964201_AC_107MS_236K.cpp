#include<iostream.h>

void main()
{
	int iStumpNum,iStump[51000],i,j;
	cin>>iStumpNum;
	for(i=0;i<iStumpNum;i++)
		cin>>iStump[i];
	for(i=0;i<iStumpNum;i++)
		if(i==iStumpNum-1)
			cout<<i+1<<endl;
		else if(iStump[i]>=iStump[i+1])
		{
			cout<<i+1<<endl;
			for(j=i;j<iStumpNum;j++)
				if(iStump[j+1]>=iStump[j])
					break;
			i=j;
		}
}