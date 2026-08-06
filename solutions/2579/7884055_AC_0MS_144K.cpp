#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>

const int LINE_LEN_MAX=10,SIZE_MAX=9;
const char QUIT[]="ENDOFINPUT";

int main(){
	while(true){
		char line[LINE_LEN_MAX+1];
		scanf("%s",line);
		if(strcmp(line,QUIT)==0)
			return 0;
		int height,width;
		scanf("%d%d",&height,&width);
		char image[SIZE_MAX][SIZE_MAX+1];
		for(int row=0;row<height;row++)
			scanf("%s",image[row]);
		scanf("%s",line);
		for(int row=0;row<height-1;row++){
			for(int col=0;col<width-1;col++)
				printf("%c",(	image[row][col]		-'0'+
								image[row][col+1]	-'0'+
								image[row+1][col]	-'0'+
								image[row+1][col+1]	-'0'	)/4+'0');
			printf("\n");
		}
	}
}