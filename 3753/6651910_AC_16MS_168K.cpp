#include<cstdio>
#include<cstring>

const int SOURCE_LEN_MAX=256,KEY_LEN_MAX=16;

int SafeStrcpy2KeyWord(char* pDestBuffer,char* pSourceString,int nDestBufferSize,char* szKeyWord){
	pDestBuffer[0]='\0';
	if(szKeyWord==NULL)
		return 0;
	char* end=strstr(pSourceString,szKeyWord);
	int count;
	if(end==NULL)
		count=strlen(pSourceString);
	else
		count=end-pSourceString;
	strncpy(pDestBuffer,pSourceString,count);
	pDestBuffer[count]='\0';
	return count;
}

int main(){
	char source[SOURCE_LEN_MAX+1];
	while(scanf("%s",source)!=EOF){
		while(true){
			char key[KEY_LEN_MAX+1];
			scanf("%s",key);
			if(strcmp(key,"END")==0)
				break;
			char dest[SOURCE_LEN_MAX+1];
			int count;
			if(strcmp(key,"NULL")==0)
				count=SafeStrcpy2KeyWord(dest,source,SOURCE_LEN_MAX,NULL);
			else
				count=SafeStrcpy2KeyWord(dest,source,SOURCE_LEN_MAX,key);
			printf("%d ",count);
			if(strlen(dest)==0)
				printf("NULL\n");
			else
				printf("%s\n",dest);
		}
	}
	return 0;
}