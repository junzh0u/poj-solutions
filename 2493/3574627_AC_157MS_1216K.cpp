#include<string>
#include<map>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<set>
using namespace std;

/*#include<fstream>
ifstream fin("in.txt",ios::in);
ofstream fout("result.txt",ios::out);

#define cin fin
#define cout fout*/


string SortMiddle(string word){
	if(word.length()>3)
		sort(word.begin()+1,word.end()-1);
	return word;
}

int main(){
	int scenarioNum;
	cin>>scenarioNum;
	for(int scenario=1;scenario<=scenarioNum;scenario++){
		cout<<"Scenario #"<<scenario<<":"<<endl;
		int dictionarySize;
		cin>>dictionarySize;
		map<string,int> dictionary;
		set<string> accorded;
		while(dictionarySize-->0){
			string word;
			cin>>word;
			if(accorded.find(word)==accorded.end()){
				dictionary[SortMiddle(word)]++;
				accorded.insert(word);
			}
		}
		int sentenceNum;
		cin>>sentenceNum;
		cin.ignore();
		while(sentenceNum-->0){
			string sentence;
			getline(cin,sentence);
			istringstream buffer;
			buffer.str(sentence);
			string word;
			int count=1;
			while(buffer>>word){
				if(dictionary.find(SortMiddle(word))==dictionary.end()){
					count=0;
					break;
				}
				count*=dictionary[SortMiddle(word)];
			}
			cout<<count<<endl;
		}
		cout<<endl;
	}
	return 0;
}