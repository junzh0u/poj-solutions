#include<iostream>
#include<string>
#include<map>
#include<set>

using namespace std;

int main(){
	int caseNum;
	cin>>caseNum;
	for(int caseId=1;caseId<=caseNum;caseId++){
		int stepNum;
		cin>>stepNum;
		map<string,string> next;
		set<string> all,notSource;
		map<string,int> in;
		while(stepNum-->1){
			string from,to;
			cin>>from>>to;
			next[from]=to;
			all.insert(from);
			all.insert(to);
			notSource.insert(to);
		}
		string source;
		for(set<string>::iterator iter=all.begin();iter!=all.end();iter++){
			if(notSource.find(*iter)==notSource.end()){
				source=*iter;
				break;
			}
		}
		cout<<"Scenario #"<<caseId<<":"<<endl;
		string city=source;
		while(true){
			cout<<city<<endl;
			if(next.find(city)==next.end())
				break;
			city=next[city];
		}
		cout<<endl;
	}
	return 0;
}