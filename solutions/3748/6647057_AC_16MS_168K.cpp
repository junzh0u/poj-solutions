#include<cstdio>

void setBit(int& number,int bit,int value){
	if(((number>>bit)&1)!=value)
		number^=(1<<bit);
}

int main(){
	int r,x,y;
	scanf("%x,%d,%d",&r,&x,&y);
	setBit(r,x,0);
	setBit(r,y,1);
	setBit(r,y-1,1);
	setBit(r,y-2,0);
	printf("%x\n",r);
	return 0;
}