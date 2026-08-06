#include<iostream>
#include<string>
using namespace std;

int main(){
	while(!cin.eof()){
		string line;
		getline(cin,line);
		int pos;
		while((pos=line.find("you"))!=-1)
			line.replace(pos,3,"we");
		cout<<line<<endl;
	}
	return 0;
}