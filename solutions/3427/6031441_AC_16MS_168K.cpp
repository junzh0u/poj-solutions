#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

int main(){
	int treeNum,logLen;
	scanf("%d%d",&treeNum,&logLen);
	int *treeHeight=new int[treeNum];
	for(int i=0;i<treeNum;i++)
		scanf("%d",treeHeight+i);
	int yearWait=0,wasteMin=INT_MAX;
	for(int i=0;i<treeNum;i++){
		int wait=(logLen-treeHeight[i]%logLen)%logLen,waste=0;
		for(int j=0;j<treeNum;j++)
			waste+=(treeHeight[j]+wait)%logLen;
		if(wasteMin>waste){
			yearWait=wait;
			wasteMin=waste;
		}
		if(wasteMin==0)
			break;
	}
	printf("%d\n",yearWait);
	delete[] treeHeight;
	return 0;
}