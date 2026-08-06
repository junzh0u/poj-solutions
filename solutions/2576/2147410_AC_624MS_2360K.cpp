#include<iostream.h>
#include<string.h>

struct Flag{bool bAvail,bCount[51];};
Flag fBuild[45001];

void main()
{
	int iNum,iSum=0,i,j,k,iWeight;
	memset(fBuild,false,sizeof(fBuild));
	//fBuild[0].bAvail=fBuild[0].bCount[0]=true;
	cin>>iNum;
	for(i=0;i<iNum;i++)
	{
		cin>>iWeight;
		iSum+=iWeight;
		for(j=1;j<iSum;j++)
		{
			//if(j==iWeight)continue;
			if(fBuild[j].bAvail)
			{
				fBuild[j+iWeight].bAvail=true;
				for(k=1;k<50;k++)
					if(fBuild[j].bCount[k])
						fBuild[j+iWeight].bCount[k+1]=true;
			}
		}
		fBuild[iWeight].bAvail=fBuild[iWeight].bCount[1]=true;
	}
	/*for(i=0;i<=iSum;i++)
		if(fBuild[i].bAvail)
		{
			cout<<"# "<<i<<"\t"<<fBuild[i].bAvail<<"\t";
			for(j=1;j<=iNum;j++)
				cout<<fBuild[i].bCount[j]<<" ";
			cout<<endl;
		}*/
	for(i=iSum/2;i>=0;i--)
		if(fBuild[i].bAvail && (fBuild[i].bCount[iNum/2] || fBuild[i].bCount[iNum-iNum/2]))
			break;
	cout<<i<<" "<<iSum-i<<endl;
}