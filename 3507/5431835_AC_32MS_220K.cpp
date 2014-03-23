#include<iostream>
using namespace std;

int main(){
	while(true){
		bool end=true;
		int sum=0,max=INT_MIN,min=INT_MAX;
		for(int i=0;i<6;i++){
			int grade;
			cin>>grade;
			sum+=grade;
			if(max<grade)
				max=grade;
			if(min>grade)
				min=grade;
			if(grade!=0)
				end=false;
		}
		if(end)
			break;
		cout<<(sum-max-min)/4.0<<endl;
	}
	return 0;
}