#include<cstdio>
using namespace std;

int main(){
	int length;
	scanf("%d",&length);
	int *sequence=new int[length],*ordered=new int[length];
	for(int i=0;i<length;i++){
		scanf("%d",sequence+i);
		ordered[i]=1;
	}
	int result=0;
	for(int i=0;i<length;i++){
		for(int j=0;j<i;j++)
			if(sequence[i]>sequence[j] && ordered[i]<ordered[j]+1)
				ordered[i]=ordered[j]+1;
		if(result<ordered[i])
			result=ordered[i];
	}
	printf("%d\n",result);
	delete[] sequence;
	delete[] ordered;
	return 0;
}