#include<iostream>
#include<string>
#include<set>
using namespace std;

//#define DEBUG

string shuffle(string left,string right){
	string all;
	for(unsigned i=0;i<right.size();i++){
		all+=right[i];
		all+=left[i];
	}
	return all;
}

int shuffleTime(string left,string right,string result){
	int time=0;
	set<string> status;
	while(true){
		string all=shuffle(left,right);
		time++;
		left=all.substr(0,left.size());
		right=all.substr(left.size(),right.size());
#ifdef DEBUG
		cout<<time<<":\t"<<all<<"\t"<<left<<"\t"<<right<<endl;
#endif
		if(status.find(all)!=status.end())
			return -1;
		status.insert(all);
		if(all==result)
			return time;
	}
}

int main(){
	int caseNum;
	cin>>caseNum;
	for(int caseID=1;caseID<=caseNum;caseID++){
		int size;
		cin>>size;
		string left,right,result;
		cin>>left>>right>>result;
		cout<<caseID<<" "<<shuffleTime(left,right,result)<<endl;
	}
	return 0;
}