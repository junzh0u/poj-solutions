//#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<string>
#include<cassert>
#include<strstream>
using namespace std;

const int DEGREE_MAX=20;

class Polynomial{
public:
	Polynomial(int number=0){
		std::fill(coefficients,coefficients+DEGREE_MAX,0);
		coefficients[0]=number;
	}
	void multiplyN(){
		for(int i=DEGREE_MAX-1;i>0;i--)
			coefficients[i]=coefficients[i-1];
		coefficients[0]=0;
	}
	void multiply(int times){
		for(int i=0;i<DEGREE_MAX;i++)
			coefficients[i]*=times;
	}
	void add(Polynomial another){
		for(int i=0;i<DEGREE_MAX;i++)
			coefficients[i]+=another.coefficients[i];
	}
	string toString(){
		string result;
		for(int i=0;i<DEGREE_MAX;i++){
			if(coefficients[i]>0){
				string part;
				if(coefficients[i]>1 || i==0){
					char buffer[100];
					sprintf(buffer,"%d",coefficients[i]);
					part+=string(buffer);
				}
				if(coefficients[i]>1 && i>0)
					part+="*";
				if(i>0)
					part+="n";
				if(i>1){
					char buffer[100];
					sprintf(buffer,"^%d",i);
					part+=string(buffer);
				}
				if(result.empty())
					result=part;
				else
					result=part+"+"+result;
			}
		}
		return result;
	}
private:
	int coefficients[DEGREE_MAX];
};

Polynomial parse(){
	string word;
	Polynomial result;
	while(cin>>word){
		if(word=="BEGIN")
			return parse();
		else if(word=="LOOP"){
			cin>>word;
			Polynomial part=parse();
			if(word=="n")
				part.multiplyN();
			else{
				int times=atoi(word.c_str());
				part.multiply(times);
			}
			result.add(part);
		}
		else if(word=="OP"){
			int number;
			cin>>number;
			result.add(Polynomial(number));
		}
		else if(word=="END")
			break;
	}
	return result;
}

int main(){
	int caseNum;
	cin>>caseNum;
	for(int caseId=1;caseId<=caseNum;caseId++){
		string runtime=parse().toString();
		if(runtime.empty())
			runtime="0";
		cout<<"Program #"<<caseId<<endl<<"Runtime = "<<runtime<<endl<<endl;
	}
	return 0;
}