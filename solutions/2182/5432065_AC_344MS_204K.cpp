#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	int cowNum;
	scanf("%d",&cowNum);
	bool *used=new bool[cowNum];
	fill(used,used+cowNum,false);
	int *reverse=new int[cowNum];
	reverse[0]=0;
	for(int i=1;i<cowNum;i++)
		scanf("%d",reverse+i);
	int *sequence=new int[cowNum];
	for(int i=cowNum-1;i>=0;i--)
		for(int j=0,count=0;j<cowNum;j++)
			if(!used[j]){
				if(count==reverse[i]){
					sequence[i]=j;
					used[j]=true;
					break;
				}
				count++;
			}
	for(int i=0;i<cowNum;i++)
		printf("%d\n",sequence[i]+1);
	delete[] sequence;
	delete[] reverse;
	delete[] used;
	return 0;
}