#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
#include<string>
#include<sstream>
using namespace std;

int main(){
	map<string,int> signals;
	signals["negative"]=-1;
	map<string,int> numbers;
	numbers["zero"]=0;
	numbers["one"]=1;
	numbers["two"]=2;
	numbers["three"]=3;
	numbers["four"]=4;
	numbers["five"]=5;
	numbers["six"]=6;
	numbers["seven"]=7;
	numbers["eight"]=8;
	numbers["nine"]=9;
	numbers["ten"]=10;
	numbers["eleven"]=11;
	numbers["twelve"]=12;
	numbers["thirteen"]=13;
	numbers["fourteen"]=14;
	numbers["fifteen"]=15;
	numbers["sixteen"]=16;
	numbers["seventeen"]=17;
	numbers["eighteen"]=18;
	numbers["nineteen"]=19;
	numbers["twenty"]=20;
	numbers["thirty"]=30;
	numbers["forty"]=40;
	numbers["fifty"]=50;
	numbers["sixty"]=60;
	numbers["seventy"]=70;
	numbers["eighty"]=80;
	numbers["ninety"]=90;
	map<string,int> conjuctions;
	conjuctions["hundred"]=100;
	map<string,int> stopwords;
	stopwords["thousand"]=1000;
	stopwords["million"]=1000000;
	string line;
	while(getline(cin,line)){
		if(line.empty())
			return 0;
#ifdef _DEBUG
		cout<<"Line: "<<line<<endl;
#endif
		stringstream separator(line);
		string word;
		int signal=1,absolute=0,part=0;
		while(separator>>word){
#ifdef _DEBUG
			cout<<"Word: "<<word<<endl;
#endif
			if(signals.find(word)!=signals.end())
				signal=signals[word];
			else if(numbers.find(word)!=numbers.end())
				part+=numbers[word];
			else if(conjuctions.find(word)!=conjuctions.end())
				part*=conjuctions[word];
			else if(stopwords.find(word)!=stopwords.end()){
				part*=stopwords[word];
				absolute+=part;
				part=0;
			}
		}
		int result=signal*(absolute+part);
		cout<<result<<endl;
	}
	return 0;
}