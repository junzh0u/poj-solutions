#include<iostream.h>
#include<string.h>
#define MAX 100
#define INF 10000

void main()
{
	int iSbNum,iContactNum,i,j,k,iContactSb,iJoint[MAX][MAX],iTime,iMin,iMinNode;
	while(cin>>iSbNum)
	{
		if(iSbNum==0)break;
		memset(iJoint,INF,sizeof(iJoint));
		for(i=0;i<iSbNum;i++)
		{
			iJoint[i][i]=0;
			cin>>iContactNum;
			while(iContactNum--)
			{
				cin>>iContactSb;
				cin>>iJoint[i][iContactSb-1];
			}
		}
		/*for(i=0;i<iSbNum;i++)
			for(j=0;j<iSbNum;j++)
				for(k=0;k<iSbNum;k++)
					if(iJoint[j][i]!=0 && iJoint[i][k]!=0 && iJoint[j][k]>iJoint[j][i]+iJoint[i][k])
						iJoint[j][k]=iJoint[j][i]+iJoint[i][k];*/
		for(k=0;k<iSbNum;k++)
			for(i=0;i<iSbNum;i++)
				for(j=0;j<iSbNum;j++)
					if(iJoint[i][j]>iJoint[i][k]+iJoint[k][j])
						iJoint[i][j]=iJoint[i][k]+iJoint[k][j];
		iMin=INF;
		for(i=0;i<iSbNum;i++)
		{
			iTime=0;
			for(j=0;j<iSbNum;j++)
				if(iTime<iJoint[i][j])
					iTime=iJoint[i][j];
			if(iMin>iTime)
			{
				iMin=iTime;
				iMinNode=i;
			}
		}
		if(iMin==INF)
			cout<<"disjoint"<<endl;
		else
			cout<<iMinNode+1<<" "<<iMin<<endl;
	}
}