#include<iostream>
#include<cmath>
using namespace std;

int main(){
	double target;
	int limit;
	cin>>target>>limit;
	int dividendBest,divisorBest;
	double errorMin=100.0;
	bool extend=false;
	for(int divisor=1;divisor<=limit;divisor++){
		int dividend=divisor*target;
		if(dividend==0)
			dividend++;
		else if(dividend>limit){
			dividend=limit;
			extend=true;
		}
		double error=fabs(target-double(dividend)/divisor);
		if(errorMin>error){
			errorMin=error;
			dividendBest=dividend;
			divisorBest=divisor;
		}
		if(extend)
			break;
		dividend++;
		error=fabs(target-double(dividend)/divisor);
		if(errorMin>error){
			errorMin=error;
			dividendBest=dividend;
			divisorBest=divisor;
		}
	}
	cout<<dividendBest<<" "<<divisorBest<<endl;
	return 0;
}