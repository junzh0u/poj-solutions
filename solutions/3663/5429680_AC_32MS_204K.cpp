#include<cstdio>
#include<algorithm>
using namespace std;

int BinarySearchBigger(int value,int *array,int begin,int end){
	if(end<=begin)
		return end;
	int mid=(begin+end)/2;
	if(array[mid]<=value)
		return BinarySearchBigger(value,array,mid+1,end);
	return BinarySearchBigger(value,array,begin,mid);
}

int main(){
	int cowNum,costumeLength;
	scanf("%d%d",&cowNum,&costumeLength);
	int *cowLength=new int[cowNum];
	for(int i=0;i<cowNum;i++)
		scanf("%d",cowLength+i);
	sort(cowLength,cowLength+cowNum);
	int pairNum=0;
	for(int i=0;i<cowNum-1;i++){
		/*int end=i+1;
		while(end<cowNum && cowLength[i]+cowLength[end]<=costumeLength)
			end++;
		pairNum+=end-i-1;*/
		pairNum+=BinarySearchBigger(costumeLength-cowLength[i],cowLength,i+1,cowNum)-i-1;
	}
	printf("%d\n",pairNum);
	delete[] cowLength;
	return 0;
}