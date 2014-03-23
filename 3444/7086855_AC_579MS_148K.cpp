#include<cstdio>
#include<algorithm>

void revert(int *array,int len){
	int *result=new int[len];
	for(int pair=0;pair*2<len;pair++){
		result[2*pair]=(array[pair]+array[len/2+pair])/2;
		result[2*pair+1]=(array[pair]-array[len/2+pair])/2;
	}
	std::copy(result,result+len,array);
	delete[] result;
}

int main(){
	int len;
	while(true){
		scanf("%d",&len);
		if(len==0)
			return 0;
		int *array=new int[len];
		for(int index=0;index<len;index++)
			scanf("%d",array+index);
		for(int subLen=2;subLen<=len;subLen*=2)
			revert(array,subLen);
		for(int index=0;index<len;index++)
			printf("%d ",array[index]);
		printf("\n");
		delete[] array;
	}
}