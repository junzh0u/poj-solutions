#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
using namespace std;

const int STUDENT_NUM_MAX=10000;
const int NAME_LEN_MAX=30;

int main(){
	int studentNum;
	cin>>studentNum;cin.ignore();
	map<string,int> scores;
	for(int i=0;i<studentNum;i++){
		string name;
		getline(cin,name);
		scores[name]=0;
	}
	int examNum;
	cin>>examNum;cin.ignore();
	for(int i=0;i<examNum;i++){
		for(int j=0;j<studentNum;j++){
			string line;
			getline(cin,line);
			int divide=line.find(' ');
			int score=atoi(line.substr(0,divide).c_str());
			string name=line.substr(divide+1);
			scores[name]+=score;
		}
		vector<int> scoreList;
		for(map<string,int>::const_iterator iter=scores.begin();iter!=scores.end();iter++)
			scoreList.push_back(iter->second);
		sort(scoreList.begin(),scoreList.end(),greater<int>());
		for(int rank=0;rank<scoreList.size();rank++)
			if(scoreList[rank]==scores["Li Ming"]){
				cout<<rank+1<<endl;
				break;
			}
	}
	return 0;
}