#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<map>
using namespace std;

map<char,int> scales;
map<int,char> letters;

int valueFromStr(string str){
	int value=0,num=1;
	for(string::const_iterator iter=str.begin();iter!=str.end();iter++){
		if(isdigit(*iter))
			num=*iter-'0';
		else{
			value+=num*scales[*iter];
			num=1;
		}
	}
	return value;
}

string strFromValue(int value){
	string str="";
	for(map<int,char>::const_reverse_iterator iter=letters.rbegin();iter!=letters.rend();iter++){
		if(value>=iter->first){
			if(value/iter->first>1)
				str+=value/iter->first+'0';
			str+=iter->second;
			value%=iter->first;
		}
	}
	return str;
}

void initialize(char letter,int scale){
	scales[letter]=scale;
	letters[scale]=letter;
}

int main(){
	initialize('m',1000);
	initialize('c',100);
	initialize('x',10);
	initialize('i',1);
	int caseNum;
	cin>>caseNum;
	while(caseNum--){
		string left,right;
		cin>>left>>right;
		cout<<strFromValue(valueFromStr(left)+valueFromStr(right))<<endl;
	}
	return 0;
}