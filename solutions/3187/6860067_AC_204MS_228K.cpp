#include<iostream>
#include<algorithm>
using namespace std;

int C(int total,int chosen){
	if(chosen==0)
		return 1;
	if(total-chosen<chosen)
		return C(total,total-chosen);
	int result=1;
	for(int i=0;i<chosen;i++)
		result*=total-i;
	for(int i=1;i<=chosen;i++)
		result/=i;
	return result;
}

int sum(int size,int *array){
	int result=0;
	for(int index=0;index<size;index++)
		result+=C(size-1,index)*array[index];
	return result;
}

int main(){
	int number,finalSum;
	cin>>number>>finalSum;
	int *array=new int[number];
	for(int i=0;i<number;i++)
		array[i]=i+1;
	do{
		if(sum(number,array)==finalSum){
			for(int index=0;index<number;index++)
				cout<<array[index]<<' ';
			cout<<endl;
			break;
		}
	}while(next_permutation(array,array+number));
	return 0;
}