#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

const char YES=0,NO=1,INF=2;

bool Check(const string &A,int lengthA,const string &B,int lengthB,const string &C,char **memo){
	if(memo[lengthA][lengthB]!=INF)
		return memo[lengthA][lengthB]==YES;
	if(lengthA==0 || lengthB==0)
		memo[lengthA][lengthB]=(A.substr(0,lengthA)+B.substr(0,lengthB)==C.substr(0,lengthA+lengthB))?YES:NO;
	else{
		if(A[lengthA-1]==C[lengthA+lengthB-1] && Check(A,lengthA-1,B,lengthB,C,memo))
			memo[lengthA][lengthB]=YES;
		else if(B[lengthB-1]==C[lengthA+lengthB-1] && Check(A,lengthA,B,lengthB-1,C,memo))
			memo[lengthA][lengthB]=YES;
	}
	return memo[lengthA][lengthB]==YES;
}

int main(){
	int caseNum;
	cin>>caseNum;
	for(int caseID=1;caseID<=caseNum;caseID++){
		string A,B,C;
		cin>>A>>B>>C;
		char **memo=new char*[A.length()+1];
		for(int i=0;i<A.length()+1;i++){
			memo[i]=new char[B.length()+1];
			fill(memo[i],memo[i]+B.length()+1,INF);
		}
		printf("Data set %d: %s\n",caseID,Check(A,A.length(),B,B.length(),C,memo)?"yes":"no");
	}
	return 0;
}