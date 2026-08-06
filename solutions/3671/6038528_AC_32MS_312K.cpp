#include<cstdio>

const int COW_NUM_MAX=30000;
bool one[COW_NUM_MAX];
int oneCount[COW_NUM_MAX+1];

int main(){
	int cowNum;
	scanf("%d",&cowNum);
	oneCount[0]=0;
	for(int i=0;i<cowNum;i++){
		int group;
		scanf("%d",&group);
		one[i]=(group==1);
		oneCount[i+1]=oneCount[i]+(one[i]?1:0);
	}
	int changeNumMin=cowNum;
	for(int i=0;i<=cowNum;i++){
		int changeNum=0;
		changeNum+=i-oneCount[i];
		changeNum+=oneCount[cowNum]-oneCount[i];
		if(changeNumMin>changeNum)
			changeNumMin=changeNum;
	}
	printf("%d\n",changeNumMin);
	return 0;
}