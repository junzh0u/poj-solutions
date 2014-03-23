#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	const int X_MAX=32001;
	int starNum;
	scanf("%d",&starNum);
	int left[X_MAX*3]={0};
	int *count=new int[starNum];
	fill(count,count+starNum,0);
	for(int i=0;i<starNum;i++){
		int x,y;
		scanf("%d%d",&x,&y);
		int min=0,max=X_MAX,ptr=0,level=0;
		while(true){
			if(min>=max){
				level+=left[ptr]++;
				break;
			}
			int mid=(min+max)/2;
			if(x<=mid){
				left[ptr]++;
				ptr=2*ptr+1;
				max=mid;
			}
			else{
				level+=left[ptr];
				ptr=2*ptr+2;
				min=mid+1;
			}
		}
		//printf("%d\n",level);
		count[level]++;
	}
	for(int i=0;i<starNum;i++)
		printf("%d\n",count[i]);
	delete[] count;
	return 0;
}