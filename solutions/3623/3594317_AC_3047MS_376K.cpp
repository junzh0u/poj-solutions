#include<iostream>
#include<string>
using namespace std;

bool Compare(int left,int right,const string& content){
	if(left>=right)
		return true;
	if(content[left]!=content[right])
		return content[left]<content[right];
	return Compare(left+1,right-1,content);
}

int main(){
	string initial,result;
	int size;
	cin>>size;
	char cow;
	for(int i=0;i<size;i++){
		cin>>cow;
		initial+=cow;
	}
	int head=0,tail=size-1;
	while(size-->0)
		result+=Compare(head,tail,initial)?initial[head++]:initial[tail--];
	for(unsigned i=0;i<result.length();i+=80)
		cout<<result.substr(i,80)<<endl;
	return 0;
}