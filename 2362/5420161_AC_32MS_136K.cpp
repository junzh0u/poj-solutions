#include<cstdio>
#include<algorithm>
#include<functional>
using namespace std;

bool Check(const int sideRemain,const int lengthNeed,int lengthAlready,const int stickNum,const int *stickLength,bool *stickUsed,const int begin){
	if(sideRemain==0)
		return true;
	if(lengthAlready==lengthNeed)
		return Check(sideRemain-1,lengthNeed,0,stickNum,stickLength,stickUsed,0);
	if(lengthAlready==0){
		int first=begin;
		while(stickUsed[first])
			first++;
		stickUsed[first]=true;
		bool temp=Check(sideRemain,lengthNeed,stickLength[first],stickNum,stickLength,stickUsed,first+1);
		stickUsed[first]=false;
		return temp;
	}
	for(int i=begin;i<stickNum;i++)
		if(!stickUsed[i] && lengthAlready+stickLength[i]<=lengthNeed){
			stickUsed[i]=true;
			bool temp=Check(sideRemain,lengthNeed,lengthAlready+stickLength[i],stickNum,stickLength,stickUsed,i+1);
			stickUsed[i]=false;
			if(lengthAlready+stickLength[i]==lengthNeed)
				return temp;
			if(temp)
				return true;
		}
	return false;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int stickNum;
		scanf("%d",&stickNum);
		int *stickLength=new int[stickNum],lengthSum=0;
		for(int i=0;i<stickNum;i++){
			scanf("%d",stickLength+i);
			lengthSum+=stickLength[i];
		}
		bool possible;
		if(lengthSum%4!=0)
			possible=false;
		else{
			sort(stickLength,stickLength+stickNum,greater<int>());
			int lengthNeed=lengthSum/4;
			if(stickLength[0]>lengthNeed)
				possible=false;
			else{
				bool *stickUsed=new bool[stickNum];
				fill(stickUsed,stickUsed+stickNum,false);
				possible=Check(4,lengthNeed,0,stickNum,stickLength,stickUsed,0);
				delete[] stickUsed;
			}
		}
		printf(possible?"yes\n":"no\n");
		delete[] stickLength;
	}
	return 0;
}