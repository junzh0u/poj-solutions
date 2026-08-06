#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

int decode(char* code,int len){
	if(len==1)
		return *code-'0'+1;
	if(*code=='0')
		return decode(code+1,len-1);
	return (1<<len)-decode(code+1,len-1)+1;
}

int main(){
	int codeNum;
	scanf("%d",&codeNum);
	for(int id=1;id<=codeNum;id++){
		int codeLen;
		scanf("%d",&codeLen);
		char *code=new char[codeLen+1];
		scanf("%s",code);
		printf("Poslanec %d se posadi na sedadlo cislo %d.\n",id,decode(code,codeLen));
		delete[] code;
	}
	return 0;
}