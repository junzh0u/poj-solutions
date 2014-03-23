#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;

int main(){
    char code[50+1];
    while(true){
        scanf("%s",code);
        if(strcmp(code,"#")==0)
            break;
        int length=strlen(code);
        if(next_permutation(code,code+length))
            printf("%s\n",code);
        else
            printf("No Successor\n");
    }
    return 0;
}