#include<iostream>
#include<algorithm>
#include<functional>
using namespace std;

int DP(int* current,int* end,int difference){
	if(current==end)
		return difference;
	if(difference<*current)
		return DP(current+1,end,difference);
	return min(DP(current+1,end,difference),DP(current+1,end,difference-*current));
}
int main(){
	int cowNum,shelfHeight;
	cin>>cowNum>>shelfHeight;
	int* cowHeights=new int[cowNum];
	int difference=0-shelfHeight;
	for(int i=0;i<cowNum;i++){
		cin>>cowHeights[i];
		difference+=cowHeights[i];
	}
	cout<<DP(cowHeights,cowHeights+cowNum,difference)<<endl;
	return 0;
}