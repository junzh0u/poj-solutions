#include<cstdio>
using namespace std;

//#define DEBUG

int main(){
	int num;
	scanf("%d",&num);
	double *height=new double[num];
	int *left=new int[num],*right=new int[num];
	for(int i=0;i<num;i++){
		scanf("%lf",height+i);
		left[i]=right[i]=1;
	}
	for(int i=0;i<num;i++)
		for(int j=0;j<i;j++)
			if(height[i]>height[j] && left[i]<left[j]+1)
				left[i]=left[j]+1;
	for(int i=num-1;i>=0;i--)
		for(int j=num-1;j>i;j--)
			if(height[i]>height[j] && right[i]<right[j]+1)
				right[i]=right[j]+1;
	int max=0;
#ifdef DEBUG
	for(int i=0;i<num;i++)
		printf("%d ",left[i]);
	printf("\n");
	for(int i=0;i<num;i++)
		printf("%d ",right[i]);
	printf("\n");
#endif
	for(int i=0;i<num;i++)
		for(int j=i;j<num;j++){
			int length=left[i]+right[j];
			if(i==j)
				length--;
			if(max<length)
				max=length;
		}
	printf("%d\n",num-max);
	return 0;
}