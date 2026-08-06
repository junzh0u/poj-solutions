#include<cstdio>
#include<vector>
using namespace std;

void Print(int size,int *set,int begin,int remain,vector<int> prev){
	if(remain==0){
		for(int i=0;i<prev.size();i++)
			printf("%d ",prev[i]);
		printf("\n");
		return;
	}
	for(;begin+remain<=size;begin++){
		vector<int> next(prev);
		next.push_back(set[begin]);
		Print(size,set,begin+1,remain-1,next);
	}
}

void ListGame(int size,int *set){
	vector<int> empty;
	Print(size,set,0,6,empty);
	printf("\n");
}

int main(){
	while(true){
		int size;
		scanf("%d",&size);
		if(size==0)
			break;
		int *set=new int[size];
		for(int i=0;i<size;i++)
			scanf("%d",set+i);
		ListGame(size,set);
		delete[] set;
	}
	return 0;
}