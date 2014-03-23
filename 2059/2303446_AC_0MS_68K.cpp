#include<iostream.h>
#include<math.h>
#include<string.h>
#define HATCH_NUM_MAX 50
#define SIDE_LEN_MAX 40

int Min(int left,int right){return (left<right)?left:right;}
int Max(int left,int right){return (left>right)?left:right;}
double Distance(int* coorLeft,int* coorRight)
{return sqrt(pow(coorLeft[0]-coorRight[0],2)+pow(coorLeft[1]-coorRight[1],2));}

void main()
{
	int caseNum,sideLen,hatchNum,hatchCoor[HATCH_NUM_MAX][2],minCoor[2],maxCoor[2],minRecord[2],fastenCoor[2],leashLen,i,j;
	bool hatchPlaced[SIDE_LEN_MAX+1][SIDE_LEN_MAX+1];
	cin>>caseNum;
	while(caseNum--)
	{
		cin>>sideLen>>hatchNum;
		minCoor[0]=minCoor[1]=SIDE_LEN_MAX;
		maxCoor[0]=maxCoor[1]=0;
		memset(hatchPlaced,false,sizeof(hatchPlaced));
		for(i=0;i<hatchNum;i++)
		{
			for(j=0;j<2;j++)
			{
				cin>>hatchCoor[i][j];
				if(minCoor[j]>hatchCoor[i][j])
					minCoor[j]=hatchCoor[i][j];
				if(maxCoor[j]<hatchCoor[i][j])
					maxCoor[j]=hatchCoor[i][j];
			}
			hatchPlaced[hatchCoor[i][0]][hatchCoor[i][1]]=true;
		}
		for(i=0;i<2;i++)
		{
			minRecord[i]=minCoor[i];
			minCoor[i]=(maxCoor[i]+1)/2;
			maxCoor[i]=(minRecord[i]+sideLen)/2;
		}
		//cout<<minCoor[0]<<" "<<minCoor[1]<<"   "<<maxCoor[0]<<" "<<maxCoor[1]<<endl;
		for(fastenCoor[0]=minCoor[0];fastenCoor[0]<=maxCoor[0];fastenCoor[0]++)
			for(fastenCoor[1]=minCoor[1];fastenCoor[1]<=maxCoor[1];fastenCoor[1]++)
			{
				if(hatchPlaced[fastenCoor[0]][fastenCoor[1]])continue;
				leashLen=Min(Min(fastenCoor[0],sideLen-fastenCoor[0]),Min(fastenCoor[1],sideLen-fastenCoor[1]));
				for(i=0;i<hatchNum;i++)
					if(Distance(fastenCoor,hatchCoor[i])>leashLen)
						break;
				if(i>=hatchNum)
					goto OUT;
			}
OUT:
		if(fastenCoor[0]>maxCoor[0])
			cout<<"poodle"<<endl;
		else
			cout<<fastenCoor[0]<<" "<<fastenCoor[1]<<endl;
		//cout<<endl;
	}
}