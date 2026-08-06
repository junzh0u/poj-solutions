#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<climits>

int main(){
	int capacity;
	scanf("%d",&capacity);
	int length,width,height,surface=INT_MAX;
	for(int l=1;l*l*l<=capacity;l++){
		if(capacity%l)
			continue;
		for(int w=1;l*w*w<=capacity;w++){
			if((capacity/l)%w)
				continue;
			int h=capacity/(l*w);
			int s=2*(l*w+l*h+w*h);
			if(surface>s){
				surface=s;
				length=l;
				width=w;
				height=h;
			}
		}
	}
	printf("%d %d %d\n",length,width,height);
	return 0;
}