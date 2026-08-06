#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int binarySearch(int *array,int begin,int end,int value){
	if(begin+1==end)
		return (array[begin]>=value)?begin:(begin+1);
	int mid=(begin+end)/2;
	return (value<array[mid])?binarySearch(array,begin,mid,value):binarySearch(array,mid,end,value);
}

int main(){
	int len;
	while(scanf("%d",&len)!=EOF){
		int *array=new int[len];
		for(int i=0;i<len;i++)
			scanf("%d",array+i);
		int *stack=new int[len],end=0;
		stack[end++]=array[0];
		for(int i=1;i<len;i++){
			if(stack[end-1]<array[i])
				stack[end++]=array[i];
			else
				stack[binarySearch(stack,0,end,array[i])]=array[i];
		}
		printf("%d\n",end);
		delete[] stack;
		delete[] array;
	}
	return 0;
}