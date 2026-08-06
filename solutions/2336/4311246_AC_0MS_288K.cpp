/*
 * 2336 Ferry Loading II.cpp
 *
 *  Created on: 2008-11-2
 *      Author: pinepara
 */

#include<iostream>
using namespace std;

int main(){
	int caseNum;
	cin>>caseNum;
	while(caseNum-->0){
		int capacity,timeAcross,carNum;
		cin>>capacity>>timeAcross>>carNum;
		int timeReturn=0,tripNum=(carNum+capacity-1)/capacity;
		while(carNum>0){
			int carLoaded=(carNum-1)%capacity+1;
			carNum-=carLoaded;
			int time;
			while(carLoaded-->0)
				cin>>time;
			if(timeReturn<time)
				timeReturn=time;
			timeReturn+=2*timeAcross;
		}
		cout<<(timeReturn-timeAcross)<<" "<<tripNum<<endl;
	}
	return 0;
}