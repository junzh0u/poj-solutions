#include<iostream>
#include<algorithm>
#include<functional>
using namespace std;

int main(){
	int cowNum,shelfHeight;
	cin>>cowNum>>shelfHeight;
	int* cowHeights=new int[cowNum];
	for(int i=0;i<cowNum;i++)
		cin>>cowHeights[i];
	sort(cowHeights,cowHeights+cowNum,greater<int>());
	int currentHeight=0,size=0;
	while(currentHeight<shelfHeight)
		currentHeight+=cowHeights[size++];
	cout<<size<<endl;
	return 0;
}