#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<map>
using std::map;

int popHighestPriorityClient(map<int,int> &waitingList){
	if(waitingList.empty())
		return 0;
	map<int,int>::iterator last=--waitingList.end();
	int client=last->second;
	waitingList.erase(last);
	return client;
}

int popLowestPriorityClient(map<int,int> &waitingList){
	if(waitingList.empty())
		return 0;
	map<int,int>::iterator first=waitingList.begin();
	int client=first->second;
	waitingList.erase(first);
	return client;
}

int main(){
	map<int,int> waitingList;
	while(true){
		int request;
		scanf("%d",&request);
		switch(request){
		case 0:
			return 0;
		case 1:
			int client,priority;
			scanf("%d%d",&client,&priority);
			waitingList[priority]=client;
			break;
		case 2:
			printf("%d\n",popHighestPriorityClient(waitingList));
			break;
		case 3:
			printf("%d\n",popLowestPriorityClient(waitingList));
			break;
		}
	}
}