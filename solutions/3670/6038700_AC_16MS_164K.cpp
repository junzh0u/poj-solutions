#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int GROUP_NUM=3;

int ascendLength[GROUP_NUM]={0},decendLength[GROUP_NUM]={0};

int main(){
	int cowNum;
	scanf("%d",&cowNum);
	for(int i=0;i<cowNum;i++){
		int id;
		scanf("%d",&id);
		id--;
		ascendLength[id]++;
		for(int j=id+1;j<GROUP_NUM;j++)
			if(ascendLength[j]<ascendLength[id])
				ascendLength[j]=ascendLength[id];
		decendLength[id]++;
		for(int j=0;j<id;j++)
			if(decendLength[j]<decendLength[id])
				decendLength[j]=decendLength[id];
	}
	printf("%d\n",cowNum-std::max(ascendLength[GROUP_NUM-1],decendLength[0]));
	return 0;
}