#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int storeNum;
		scanf("%d",&storeNum);
		int min=INT_MAX,max=INT_MIN;
		while(storeNum-->0){
			int position;
			scanf("%d",&position);
			if(min>position)
				min=position;
			if(max<position)
				max=position;
		}
		printf("%d\n",2*(max-min));
	}
	return 0;
}