#include<iostream>
#include<cmath>
using namespace std;

int main(){
	int caseNum;
	cin>>caseNum;
	while(caseNum-->0){
		int blockNum;
		cin>>blockNum;
		int paperMin=10000;
		for(int height=1;height<=pow(blockNum+1.0,1.0/3.0);height++)
			if(blockNum%height==0){
				int area=blockNum/height;
				for(int width=1;width<=sqrt(area+1.0);width++)
					if(area%width==0){
						int length=area/width;
						int paper=2*(height*length+height*width+length*width);
						if(paperMin>paper)
							paperMin=paper;
					}
			}
		cout<<paperMin<<endl;
	}
	return 0;
}