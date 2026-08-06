#include<cstdio>

int main(){
	int cowNum;
	scanf("%d",&cowNum);
	int *height=new int[cowNum];
	for(int i=0;i<cowNum;i++)
		scanf("%d",height+i);
	int *count=new int[cowNum];
	unsigned sum=0;
	for(int i=cowNum-1;i>=0;i--){
		count[i]=0;
		int next=i+1;
		while(next<cowNum && height[i]>height[next]){
			count[i]+=count[next]+1;
			next+=count[next]+1;
		}
		sum+=count[i];
	}
	delete[] height;
	delete[] count;
	printf("%u\n",sum);
	return 0;
}