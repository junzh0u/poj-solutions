#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	int length;
	scanf("%d",&length);
	int *sequence=new int[length];
	for(int i=0;i<length;i++)
		scanf("%d",sequence+i);
	sort(sequence,sequence+length);
	if(length%2==0)
		printf("%.1f\n",((double)sequence[length/2]+sequence[length/2-1])/2.0);
	else
		printf("%.1f\n",(double)sequence[length/2]);
	delete[] sequence;
	return 0;
}