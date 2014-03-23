#include<iostream>
#include<string>
#include<map>
using namespace std;

int main(){
	int candidateNum;
	cin>>candidateNum;
	while(cin.get()!='\n');
	map<string,string> partyOfName;
	for(int i=0;i<candidateNum;i++){
		string name,party;
		getline(cin,name);
		getline(cin,party);
		partyOfName[name]=party;
	}
	int voteNum;
	cin>>voteNum;
	while(cin.get()!='\n');
	map<string,int> voteOfName;
	for(int i=0;i<voteNum;i++){
		string name;
		getline(cin,name);
		voteOfName[name]++;
	}
	map<string,int>::iterator winner=voteOfName.end();
	bool tie=false;
	for(map<string,int>::iterator iter=voteOfName.begin();iter!=voteOfName.end();iter++){
		if(winner==voteOfName.end() || winner->second<iter->second){
			winner=iter;
			tie=false;
		}
		else if(winner->second==iter->second)
			tie=true;
	}
	if(tie)
		cout<<"tie"<<endl;
	else
		cout<<partyOfName[winner->first]<<endl;
	return 0;
}