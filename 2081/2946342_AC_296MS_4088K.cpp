#include<iostream>
using namespace std;

int main(){
	const int SIZE=4000000;
	bool *record=new bool[SIZE];
	int num;
	while(cin>>num && num!=-1){
		memset(record,false,sizeof(bool)*SIZE);
		int recaman=0;
		for(int i=1;i<=num;i++){
			record[recaman]=true;
			if(recaman>i && !record[recaman-i])
				recaman-=i;
			else
				recaman+=i;
		}
		cout<<recaman<<endl;
	}
}