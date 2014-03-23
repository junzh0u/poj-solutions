#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int SYMBOL_NUM_MAX=26,ID_LEN_MAX=2000;

int symbolNum,cost[SYMBOL_NUM_MAX],idLen;
char id[ID_LEN_MAX+1];
int costMin[ID_LEN_MAX][ID_LEN_MAX]={0};

int main(){
	scanf("%d%d%s",&symbolNum,&idLen,id);
	for(int i=0;i<symbolNum;i++){
		char symbol;
		int add,del;
		getchar();
		scanf("%c%d%d",&symbol,&add,&del);
		cost[symbol-'a']=std::min(add,del);
	}
	for(int step=1;step<idLen;step++){
		for(int first=0,last=step;last<idLen;first++,last++){
			if(id[first]==id[last])
				costMin[first][last]=costMin[first+1][last-1];
			else
				costMin[first][last]=std::min(cost[id[first]-'a']+costMin[first+1][last],cost[id[last]-'a']+costMin[first][last-1]);
		}
	}
	printf("%d\n",costMin[0][idLen-1]);
	return 0;
}