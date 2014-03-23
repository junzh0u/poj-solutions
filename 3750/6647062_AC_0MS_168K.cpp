#include<cstdio>

const int CHILD_NUM_MAX=64,NAME_LEN_MAX=15;

int main(){
	int childNum;
	scanf("%d",&childNum);
	char names[CHILD_NUM_MAX][NAME_LEN_MAX+1];
	for(int childID=0;childID<childNum;childID++)
		scanf("%s",names+childID);
	int begin,outCount;
	scanf("%d,%d",&begin,&outCount);
	begin--;
	bool out[CHILD_NUM_MAX]={false};
	int remainNum=childNum;
	while(remainNum){
		for(int childID=begin,count=0;;childID=(childID+1)%childNum){
			if(out[childID])
				continue;
			count++;
			if(count==outCount){
				printf("%s\n",names[childID]);
				out[childID]=true;
				remainNum--;
				begin=childID;
				break;
			}
		}
	}
	return 0;
}