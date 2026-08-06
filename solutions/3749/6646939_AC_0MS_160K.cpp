#include<cstdio>
#include<cstring>
#include<cctype>

int main(){
	char line[220];
	while(true){
		gets(line);
		if(strcmp(line,"ENDOFINPUT")==0)
			break;
		gets(line);
		for(int i=0;line[i]!='\0';i++){
			if(isalpha(line[i])){
				if(line[i]>='F')
					putchar(line[i]-5);
				else
					putchar(line[i]+21);
			}
			else
				putchar(line[i]);
		}
		putchar('\n');
		gets(line);
	}
	return 0;
}