#include<cstdio>

int main(){
	char c;
	bool first=true;
	while(true){
		c=getchar();
		if(c==EOF)
			break;
		if(c=='\"'){
			if(first){
				putchar('`');
				putchar('`');
			}
			else{
				putchar('\'');
				putchar('\'');
			}
			first=!first;
		}
		else
			putchar(c);
	}
	return 0;
}