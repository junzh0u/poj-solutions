#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cstring>
#include<algorithm>
#include<functional>

const int LETTER_NUM_MAX=12;
const char END[]="END";

int target;
char set[LETTER_NUM_MAX+1];

bool decode(){
	int len=strlen(set);
	std::sort(set,set+len,std::greater<char>());
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++){
			if(j==i)
				continue;
			for(int k=0;k<len;k++){
				if(k==i || k==j)
					continue;
				for(int l=0;l<len;l++){
					if(l==i || l==j || l==k)
						continue;
					for(int m=0;m<len;m++){
						if(m==i || m==j || m==k || m==l)
							continue;
						int v=set[i]-'A'+1,
							w=set[j]-'A'+1,
							x=set[k]-'A'+1,
							y=set[l]-'A'+1,
							z=set[m]-'A'+1;
						if(v-w*w+x*x*x-y*y*y*y+z*z*z*z*z==target){
							printf("%c%c%c%c%c\n",set[i],set[j],set[k],set[l],set[m]);
							return true;
						}
					}
				}
			}
		}
	return false;
}

int main(){
	while(true){
		scanf("%d%s",&target,set);
		if(target==0 && strcmp(set,END)==0)
			return 0;
		if(!decode())
			printf("no solution\n");
	}
}