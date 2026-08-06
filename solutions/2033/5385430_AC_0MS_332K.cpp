#include<iostream>
#include<string>
using namespace std;

//#define DEBUG

int main(){
	while(true){
		string encryption;
		cin>>encryption;
		if(encryption=="0")
			break;
		int *possibleDecodeNum=new int[encryption.size()+1];
		possibleDecodeNum[encryption.size()]=1;
		for(int i=encryption.size()-1;i>=0;i--){
			if(encryption[i]=='0')
				possibleDecodeNum[i]=0;
			else{
				possibleDecodeNum[i]=possibleDecodeNum[i+1];
				if(i+1<encryption.size() && (encryption[i]-'0')*10+encryption[i+1]-'0'<=26)
					possibleDecodeNum[i]+=possibleDecodeNum[i+2];
			}
		}
#ifdef DEBUG
		for(int i=0;i<encryption.size();i++)
			cout<<possibleDecodeNum[i]<<" ";
		cout<<endl;
#endif
		cout<<possibleDecodeNum[0]<<endl;
	}
	return 0;
}