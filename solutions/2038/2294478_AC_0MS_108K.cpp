#include<iostream>
#include<algorithm>
using namespace std;
#define MAX 100
#define TEAMNUM 5
#define FACTORIAL 120

int CalcDistance(char* rankingLeft,char* rankingRight)
{
	int distance=0,positionLeft[TEAMNUM],positionRight[TEAMNUM],i,j;
	for(i=0;i<TEAMNUM;i++)
	{
		positionLeft[rankingLeft[i]-'A']=i;
		positionRight[rankingRight[i]-'A']=i;
	}
	for(i=0;i<TEAMNUM-1;i++)for(j=i+1;j<TEAMNUM;j++)
		distance+=((positionLeft[i]>positionLeft[j])!=(positionRight[i]>positionRight[j]));
	return distance;
}

void main()
{
	char ranking[MAX][TEAMNUM+1],permutation[FACTORIAL][TEAMNUM+1]={"ABCDE"};
	int rankNum,distanceSum,distanceSumMin,medianRank,i,j;
	for(i=1;i<FACTORIAL;i++)
	{
		strcpy(permutation[i],permutation[i-1]);
		next_permutation(permutation[i],permutation[i]+TEAMNUM);
	}
	while(cin>>rankNum)
	{
		if(rankNum==0)break;
		for(i=0;i<rankNum;i++)cin>>ranking[i];
		for(i=0,distanceSumMin=1000000,medianRank=0;i<FACTORIAL;i++)
		{
			for(j=0,distanceSum=0;j<rankNum;j++)
				distanceSum+=CalcDistance(permutation[i],ranking[j]);
			if(distanceSumMin>distanceSum)
				{distanceSumMin=distanceSum;medianRank=i;}
		}
		cout<<permutation[medianRank]<<" is the median ranking with value "<<distanceSumMin<<"."<<endl;
	}
}