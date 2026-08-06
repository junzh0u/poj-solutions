#include<iostream.h>
#include<math.h>

int iMemory[33000][4]={0};

void main()
{
	int i,j,k,iSum;
	//for(i=1;i<33000;i++)if(pow((int)sqrt(i),2)==i)iMemory[i][0]=1;
	for(i=1;i*i<33000;i++)
	{
		iMemory[i*i][0]=1;
		for(j=1;j+i*i<33000;j++)
			for(k=1;k<4;k++)
				iMemory[j+i*i][k]+=iMemory[j][k-1];
	}
	while(cin>>iSum)
	{
		if(iSum==0)break;
		cout<<iMemory[iSum][0]+iMemory[iSum][1]+iMemory[iSum][2]+iMemory[iSum][3]<<endl;
	}
}