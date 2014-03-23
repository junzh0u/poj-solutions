#include<iostream>
using namespace std;

const int VILLAGE_NUM_MAX=300,POST_NUM_MAX=30,INF=10000000;
int villageNum,postNum,position[VILLAGE_NUM_MAX];
int postDistance[VILLAGE_NUM_MAX][VILLAGE_NUM_MAX]={0},memory[VILLAGE_NUM_MAX][POST_NUM_MAX];

int postDistanceSumMin(int villageNum,int postNum){
	if(postNum==1)
		return postDistance[0][villageNum-1];
	if(memory[villageNum-1][postNum-1]!=INF)
		return memory[villageNum-1][postNum-1];
	int sumMin=INF;
	for(int manage=1;manage<villageNum-postNum+1;manage++){
		int sum=postDistanceSumMin(villageNum-manage,postNum-1)+postDistance[villageNum-manage][villageNum-1];
		if(sumMin>sum)
			sumMin=sum;
	}
	return memory[villageNum-1][postNum-1]=sumMin;
}

int main(){
	cin>>villageNum>>postNum;
	for(int i=0;i<villageNum;i++)
		cin>>position[i];
	for(int begin=0;begin<villageNum;begin++)
		for(int last=begin;last<villageNum;last++){
			int mid=(begin+last)/2;
			//postDistance[begin][last]=0;
			for(int i=begin;i<=last;i++)
				postDistance[begin][last]+=abs(position[mid]-position[i]);
		}
	for(i=0;i<villageNum;i++)
		for(int j=0;j<postNum;j++)
			memory[i][j]=INF;
	cout<<postDistanceSumMin(villageNum,postNum)<<endl;
	return 0;
}