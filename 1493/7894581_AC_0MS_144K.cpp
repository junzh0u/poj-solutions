#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int WIDTH=25;

int main(){
	while(true){
		int height;
		scanf("%d",&height);
		if(!height)
			return 0;
		char line[WIDTH+1];
		gets(line);
		int spaceSum=0,spaceMin=INT_MAX;
		for(int row=0;row<height;row++){
			gets(line);
			int space=std::count(line,line+strlen(line),' ');
			spaceSum+=space;
			if(spaceMin>space)
				spaceMin=space;
		}
		printf("%d\n",spaceSum-spaceMin*height);
	}
}