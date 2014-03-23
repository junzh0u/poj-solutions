#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int MEMBER_NUM_MAX=100;

struct Member{
	int id,childNum,children[MEMBER_NUM_MAX-1],discover,finish;
	bool root;
};

int memberNum;
Member members[MEMBER_NUM_MAX];
int time;

void dfs(int index){
	members[index].discover=time++;
	for(int i=0;i<members[index].childNum;i++)
		if(members[members[index].children[i]].discover==INT_MAX)
			dfs(members[index].children[i]);
	members[index].finish=time++;
}

bool compareByFinish(const Member &left,const Member &right){
	return left.finish>right.finish;
}

int main(){
	scanf("%d",&memberNum);
	for(int i=0;i<memberNum;i++){
		members[i].id=i+1;
		members[i].childNum=0;
		members[i].discover=members[i].finish=INT_MAX;
		members[i].root=true;
	}
	for(int i=0;i<memberNum;i++){
		while(true){
			int child;
			scanf("%d",&child);
			if(!child)
				break;
			members[i].children[members[i].childNum++]=child-1;
			members[child-1].root=false;
		}
	}
	time=0;
	for(int i=0;i<memberNum;i++)
		if(members[i].root)
			dfs(i);
	std::sort(members,members+memberNum,compareByFinish);
	for(int i=0;i<memberNum;i++)
		printf("%d ",members[i].id);
	printf("\n");
	return 0;
}