#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

int main(){
	int personNum,treeNum;
	cin>>personNum>>treeNum;
	string person[100];
	for(int i=0;i<personNum;i++)
		person[i].append(treeNum,'0');
	int personHear,treeHeard;
	while(cin>>personHear>>treeHeard)
		person[personHear-1][treeHeard-1]='1';
	sort(person,person+personNum);
	int opinionNum=1;
	for(i=1;i<personNum;i++)
		if(person[i]!=person[i-1])
			opinionNum++;
	cout<<opinionNum<<endl;
	delete[] person;
}