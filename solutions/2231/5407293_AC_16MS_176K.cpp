#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	int cowNum;
	scanf("%d",&cowNum);
	int *location=new int[cowNum];
	for(int i=0;i<cowNum;i++)
		scanf("%d",location+i);
	sort(location,location+cowNum);
	long long volumeSum=0;
	for(int i=0;i<cowNum;i++)
		volumeSum+=(long long)location[i]*(2*i-cowNum+1);
	volumeSum*=2;
	printf("%lld\n",volumeSum);
	return 0;
}