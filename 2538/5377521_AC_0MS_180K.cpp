#include<iostream>
#include<map>
#include<string>
using namespace std;

int main(){
	map<char,char> correct;
	correct['1']='`';
	correct['2']='1';
	correct['3']='2';
	correct['4']='3';
	correct['5']='4';
	correct['6']='5';
	correct['7']='6';
	correct['8']='7';
	correct['9']='8';
	correct['0']='9';
	correct['-']='0';
	correct['=']='-';
	//correct['Q']='\t';
	correct['W']='Q';
	correct['E']='W';
	correct['R']='E';
	correct['T']='R';
	correct['Y']='T';
	correct['U']='Y';
	correct['I']='U';
	correct['O']='I';
	correct['P']='O';
	correct['[']='P';
	correct[']']='[';
	correct['\\']=']';
	correct['S']='A';
	correct['D']='S';
	correct['F']='D';
	correct['G']='F';
	correct['H']='G';
	correct['J']='H';
	correct['K']='J';
	correct['L']='K';
	correct[';']='L';
	correct['\'']=';';
	correct['X']='Z';
	correct['C']='X';
	correct['V']='C';
	correct['B']='V';
	correct['N']='B';
	correct['M']='N';
	correct[',']='M';
	correct['.']=',';
	correct['/']='.';
	correct[' ']=' ';
	while(cin){
		string line;
		getline(cin,line);
		for(int i=0;i<line.length();i++)
			line[i]=correct[line[i]];
		cout<<line<<endl;
	}
	return true;
}