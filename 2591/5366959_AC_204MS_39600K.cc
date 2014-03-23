#include<cstdio>
#include<algorithm>
using namespace std;

const int LIMIT=10000000;
int set[LIMIT]={1},two=0,three=0;

int main(){
    for(int i=1;i<LIMIT;i++){
        while(2*set[two]+1<=set[i-1]){
            two++;
        }
        while(3*set[three]+1<=set[i-1]){
            three++;
        }
        set[i]=min(2*set[two]+1,3*set[three]+1);
    }
    int n;
    while(scanf("%d",&n)!=EOF){
        printf("%d\n",set[n-1]);
    }
    return 0;
}