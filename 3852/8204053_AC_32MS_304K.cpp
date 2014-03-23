#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<set>
using namespace std;

bool hasEmpty(vector<string> &words){
	for(unsigned i=0;i<words.size();i++)
		if(words[i].empty())
			return true;
	return false;
}

bool hasDuplicate(vector<string> &words){
	set<string> uniqueWords;
	for(unsigned i=0;i<words.size();i++){
		if(uniqueWords.find(words[i])!=uniqueWords.end())
			return true;
		uniqueWords.insert(words[i]);
	}
	return false;
}

void leftDelete(vector<string> &words){
	for(unsigned i=0;i<words.size();i++)
		words[i]=words[i].substr(1);
}

int main(){
	while(true){
		int wordNum;
		cin>>wordNum;
		if(!wordNum)
			return 0;
		vector<string> words;
		while(wordNum--){
			string word;
			cin>>word;
			words.push_back(word);
		}
		int step=-1;
		while(!hasDuplicate(words) && !hasEmpty(words)){
			leftDelete(words);
			step++;
		}
		cout<<step<<endl;
	}
}