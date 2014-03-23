#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>
#include<algorithm>

int main(){
	while(true){
		char line[25];
		scanf("%s",line);
		std::reverse(line,line+strlen(line));
		int a,b,sum;
		sscanf(line,"%d=%d+%d",&sum,&a,&b);
		printf((a+b==sum)?"True\n":"False\n");
		if(!strcmp(line,"0=0+0"))
			return 0;
	}
}