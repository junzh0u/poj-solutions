#include<cstdio>

__int64 Merge(int* array,int begin,int mid,int end){
	int* temp=new int[end-begin];
	int ptr=0,left=begin,right=mid;
	__int64 count=0;
	while(left<mid && right<end){
		if(array[left]<array[right])
			temp[ptr++]=array[left++];
		else{
			temp[ptr++]=array[right++];
			count+=mid-left;
		}
	}
	while(left<mid)
		temp[ptr++]=array[left++];
	while(right<end)
		temp[ptr++]=array[right++];
	for(ptr=0;ptr<end-begin;ptr++)
		array[begin+ptr]=temp[ptr];
	delete[] temp;
	return count;
}

__int64 MergeSort(int* array,int begin,int end){
	if(begin+1>=end)
		return 0;
	int mid=(begin+end)/2;
	return MergeSort(array,begin,mid)+MergeSort(array,mid,end)+Merge(array,begin,mid,end);
}

int main(){
	int size;
	while(scanf("%d",&size) && size>0){
		int* array=new int[size];
		for(int i=0;i<size;i++)
			scanf("%d",&array[i]);
		printf("%I64d\n",MergeSort(array,0,size));
		delete[] array;
	}
	return 0;
}