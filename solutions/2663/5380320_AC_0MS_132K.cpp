#include<cstdio>
#include<algorithm>
using namespace std;

const int LIMIT=30,INF=-1;

int memory[LIMIT];

int SchemeNum(const int length){
	if(length==0)
		return 1;
	if(length%2!=0)
		return 0;
	if(memory[length-1]!=INF)
		return memory[length-1];
	memory[length-1]=3*SchemeNum(length-2);
	for(int j=length-4;j>=0;j-=2)
		memory[length-1]+=SchemeNum(j)*2;
	return memory[length-1];
}

int main(){
	fill(memory,memory+LIMIT,INF);
	while(true){
		int length;
		scanf("%d",&length);
		if(length==-1)
			return 0;
		printf("%d\n",SchemeNum(length));
	}
}