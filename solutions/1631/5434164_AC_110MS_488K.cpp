#include<cstdio>

int BinarySearchGreater(int *array,int begin,int end,int value){
	if(end-begin<=1)
		return end;
	int mid=(begin+end)/2;
	if(array[mid]<=value)
		return BinarySearchGreater(array,mid,end,value);
	return BinarySearchGreater(array,begin,mid,value);
}

int LongestIncreasingSubsequenceLength(const int* array,const int size){
	int *min=new int[size+1],length=0;
	min[0]=0;
	for(int i=0;i<size;i++){
		if(array[i]>=min[length])
			min[++length]=array[i];
		else
			min[BinarySearchGreater(min,0,length+1,array[i])]=array[i];
	}
	delete[] min;
	return length;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int portNum;
		scanf("%d",&portNum);
		int *connect=new int[portNum];
		for(int i=0;i<portNum;i++)
			scanf("%d",connect+i);
		printf("%d\n",LongestIncreasingSubsequenceLength(connect,portNum));
		delete[] connect;
	}
	return 0;
}