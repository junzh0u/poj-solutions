#include<iostream>
using namespace std;

int main(){
	int caseNum;
	cin>>caseNum;
	for(int caseId=1;caseId<=caseNum;caseId++){
		int a,b,c,d;
		cin>>a>>b>>c>>d;
		cout<<"Scenario #"<<caseId<<":"<<endl;
		cout<<((a+b-c-d)%2!=0)<<endl<<endl;
	}
	return 0;
}