#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<cassert>
using namespace std;

const string END_OF_INPUT="*";
char EMPTY=' ';

vector<string> diagram;
int beginRow,beginCol;

bool ReadDiagram(){
	diagram.clear();
	string line;
	while(getline(cin,line) && line!=END_OF_INPUT){
		for(int col=0;col<line.length();col++)
			if(line[col]=='?'){
				beginRow=diagram.size();
				beginCol=col;
			}
		diagram.push_back(line);
	}
	return !diagram.empty();
}

char& At(int row,int col){
	if(row>=0 && row<diagram.size() && col>=0 && col<diagram[row].length())
		return diagram[row][col];
	return EMPTY;
}

bool Callback(int row,int col,string state){
	char ch=At(row,col);
	assert(ch!=' ');
	At(row,col)=' ';
	switch(ch){
		case '?':case '+':{
			if(At(row-1,col)=='|')
				return Callback(row-1,col,state);
			if(At(row+1,col)=='|')
				return Callback(row+1,col,state);
			if(At(row,col-1)=='-')
				return Callback(row,col-1,state);
			if(At(row,col+1)=='-')
				return Callback(row,col+1,state);
			assert(false);
				 }
		case '-':{
			if(At(row,col-1)!=' ')
				return Callback(row,col-1,state);
			return Callback(row,col+1,state);
				 }
		case '|':{
			if(At(row-1,col)!=' ')
				return Callback(row-1,col,state);
			return Callback(row+1,col,state);
				 }
		case 'o':
			return !Callback(row,col-1,state);
		case ')':
			return Callback(row-1,col-3,state) && Callback(row+1,col-3,state);
		case '>':
			return Callback(row-1,col-3,state) || Callback(row+1,col-3,state);
		default:
			return state[ch-'A']=='1';
	}
}

bool RunState(){
	string state;
	if(getline(cin,state) && state!=END_OF_INPUT){
		vector<string> backup(diagram);
		cout<<(Callback(beginRow,beginCol,state)?1:0)<<endl;
		diagram=backup;
		return true;
	}
	return false;
}
		
int main(){
	while(ReadDiagram()){
		while(RunState());
		cout<<endl;
	}
	return 0;
}