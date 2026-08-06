#include<cstdio>

const int BFS_DEQUE_SIZE_MAX=100000;

int main(){
	int handleState[BFS_DEQUE_SIZE_MAX]={0};
	for(int row=0;row<4;row++){
		for(int col=0;col<4;col++){
			char c=getchar();
			if(c=='+'){
				int mask=(1<<(row*4+col));
				handleState[0]|=mask;
			}
		}
		getchar();
	}
	int operationMask[16];
	for(int row=0;row<4;row++)
		for(int col=0;col<4;col++){
			int mask=0;
			for(int i=0;i<4;i++){
				mask|=(1<<(row*4+i));
				mask|=(1<<(i*4+col));
			}
			operationMask[row*4+col]=mask;
		}
	int operationState[BFS_DEQUE_SIZE_MAX]={0},head=0,tail=1;
	bool operationPool[BFS_DEQUE_SIZE_MAX]={false};
	operationPool[0]=true;
	while(head<tail){
		if(!handleState[head])
			break;
		for(int operation=0;operation<16;operation++){
			operationState[tail]=(operationState[head]|(1<<operation));
			if(operationPool[operationState[tail]])
				continue;
			operationPool[operationState[tail]]=true;
			handleState[tail]=(handleState[head]^operationMask[operation]);
			tail++;
		}
		head++;
	}
	int count=0;
	for(int operation=0;operation<16;operation++)
		if(operationState[head]&(1<<operation))
			count++;
	printf("%d\n",count);
	for(int operation=0;operation<16;operation++)
		if(operationState[head]&(1<<operation))
			printf("%d %d\n",operation/4+1,operation%4+1);
	
	return 0;
}