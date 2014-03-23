#include<cstdio>

int main(){
	char letter;
	while((letter=getchar())!='#'){
		switch(letter){
			case ' ':
				printf("%%20");break;
			case '!':
				printf("%%21");break;
			case '$':
				printf("%%24");break;
			case '%':
				printf("%%25");break;
			case '(':
				printf("%%28");break;
			case ')':
				printf("%%29");break;
			case '*':
				printf("%%2a");break;
			default:
				putchar(letter);break;
		}
	}
	return 0;
}