#include<cstdio>
#include<string>

const int MESSAGE_LENGTH_MAX=70;

int code(char letter){
	if(letter=='_')
		return 0;
	if(letter>='a' && letter<='z')
		return letter-'a'+1;
	if(letter=='.')
		return 27;
	return -1;
}

char letter(int code){
	if(code==0)
		return '_';
	if(code>=1 && code<=26)
		return 'a'+code-1;
	if(code==27)
		return '.';
	return '\0';
}

int main(){
	while(true){
		int key;
		scanf("%d",&key);
		if(key==0)
			break;
		char twisted[MESSAGE_LENGTH_MAX+1],untwisted[MESSAGE_LENGTH_MAX+1];
		scanf("%s",twisted);
		int length=strlen(twisted);
		for(int i=0;i<length;i++)
			untwisted[(key*i)%length]=letter((code(twisted[i])+i)%28);
		untwisted[length]='\0';
		printf("%s\n",untwisted);
	}
	return 0;
}