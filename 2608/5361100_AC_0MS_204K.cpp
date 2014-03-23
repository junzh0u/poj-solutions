#include<cstdio>

int Soundex(char letter){
	switch(letter){
		case 'B':
		case 'F':
		case 'P':
		case 'V':
			return 1;
		case 'C':
		case 'G':
		case 'J':
		case 'K':
		case 'Q':
		case 'S':
		case 'X':
		case 'Z':
			return 2;
		case 'D':
		case 'T':
			return 3;
		case 'L':
			return 4;
		case 'M':
		case 'N':
			return 5;
		case 'R':
			return 6;
		default:
			return 0;
	}
}

int main(){
	const int MAX_LEN=20;
	char word[MAX_LEN+1];
	while(scanf("%s",word)!=EOF){
		int last=0;
		for(int i=0;word[i]!='\0';i++){
			int digit=Soundex(word[i]);
			if(digit==last)
				continue;
			last=digit;
			if(digit!=0)
				printf("%d",digit);
		}
		printf("\n");
	}
	return 0;
}