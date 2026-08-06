#include<iostream>
#include<string>
#include<set>
#include<algorithm>
using namespace std;

typedef set<string> SS;

void main()
{
	int iCaseNum,i;
	string strArrive,strStorage[2];
	SS ssDeparture;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		ssDeparture.clear();
		cin>>strArrive;
		for(i=1;i<strArrive.length();i++)
		{
			strStorage[0].assign(strArrive.begin(),&strArrive[i]);
			strStorage[1].assign(&strArrive[i],strArrive.end());
			ssDeparture.insert(strStorage[0]+strStorage[1]);
			ssDeparture.insert(strStorage[1]+strStorage[0]);
			reverse(strStorage[0].begin(),strStorage[0].end());
			ssDeparture.insert(strStorage[0]+strStorage[1]);
			ssDeparture.insert(strStorage[1]+strStorage[0]);
			reverse(strStorage[1].begin(),strStorage[1].end());
			ssDeparture.insert(strStorage[0]+strStorage[1]);
			ssDeparture.insert(strStorage[1]+strStorage[0]);
			reverse(strStorage[0].begin(),strStorage[0].end());
			ssDeparture.insert(strStorage[0]+strStorage[1]);
			ssDeparture.insert(strStorage[1]+strStorage[0]);
		}
		cout<<ssDeparture.size()<<endl;
	}
}