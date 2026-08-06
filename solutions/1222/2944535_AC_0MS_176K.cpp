#include<iostream>
using namespace std;

string BitStr(int value){
	string bitset;
	for(int bit=0;bit<6;bit++){
		bitset+=char((value&1)+'0');
		bitset+=" ";
		value=value>>1;
	}
	return bitset;
}

int main(){
	int caseNum;
	cin>>caseNum;
	for(int ID=1;ID<=caseNum;ID++){
		int map[5]={0};
		for(int row=0;row<5;row++)
			for(int col=0;col<6;col++){
				int light;
				cin>>light;
				map[row]|=light<<col;
			}
		int operation[5];
		for(int attempt=0;attempt<(1<<6);attempt++){
			int after[5];
			for(int row=0;row<5;row++)
				after[row]=map[row];
			for(row=0;row<5;row++){
				if(row==0)
					operation[row]=attempt;
				else
					operation[row]=after[row-1];
				after[row]^=operation[row];
				after[row]^=(operation[row]<<1)&63;
				after[row]^=operation[row]>>1;
				if(row<4)
					after[row+1]^=operation[row];
			}
			if(after[4]==0)
				break;
		}
		cout<<"PUZZLE #"<<ID<<endl;
		for(row=0;row<5;row++){
			for(int bit=0;bit<6;bit++){
				cout<<(operation[row]&1)<<" ";
				operation[row]>>=1;
			}
			cout<<endl;
		}
	}
	return 0;
}