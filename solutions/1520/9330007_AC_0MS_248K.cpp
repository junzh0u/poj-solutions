#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

bool isNumber(string word){
	if(word.empty())
		return false;
	if(word[0]!='-' && !isdigit(word[0]))
		return false;
	for(int i=1;i<word.length();i++)
		if(!isdigit(word[i]))
			return false;
	return true;
}

bool lessIgnoredCase(string left,string right){
	for(int i=0;i<left.size();i++)
		left[i]=tolower(left[i]);
	for(int i=0;i<right.size();i++)
		right[i]=tolower(right[i]);
	return left<right;
}

int main(){
	string word;
	while(true){
		vector<bool> series;
		vector<string> words;
		vector<int> numbers;
		while(true){
			cin>>word;
			bool end=(*word.rbegin()=='.');
			word=word.substr(0,word.length()-1);
			bool isNum=isNumber(word);
			series.push_back(isNum);
			if(isNum)
				numbers.push_back(atoi(word.c_str()));
			else
				words.push_back(word);
			if(end){
				if(series.size()==1 && numbers.empty() && words[0].empty())
					return 0;
				sort(words.begin(),words.end(),lessIgnoredCase);
				sort(numbers.begin(),numbers.end());
				vector<string>::iterator wordIter=words.begin();
				vector<int>::iterator numberIter=numbers.begin();
				for(int i=0;i<series.size();i++){
					if(i)
						cout<<", ";
					if(series[i])
						cout<<*(numberIter++);
					else
						cout<<*(wordIter++);
				}
				cout<<"."<<endl;
				break;
			}
		}
	}
}