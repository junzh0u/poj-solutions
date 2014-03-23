#include<cstdio>
#include<algorithm>

const int COW_NUM_MAX=100000,RANGE_MAX=10000;

int main(){
	int cowNum,range;
	scanf("%d%d",&cowNum,&range);
	int breed[COW_NUM_MAX],groupNum=0,groupSize=0;
	bool group[RANGE_MAX];
	memset(group,false,sizeof(bool)*range);
	for(int i=0;i<cowNum;i++){
		scanf("%d",breed+i);
		if(!group[breed[i]-1]){
			group[breed[i]-1]=true;
			groupSize++;
			if(groupSize==range){
				groupNum++;
				memset(group,false,sizeof(bool)*range);
				groupSize=0;
			}
		}
	}
	printf("%d\n",groupNum+1);
	return 0;
}