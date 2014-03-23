#include<cstdio>

//#define DEBUG

int main(){
    const int FACTOR_NUM=3;
    int factors[FACTOR_NUM],factorCoef[FACTOR_NUM]={0,0,0};
    for(int i=0;i<FACTOR_NUM;i++){
        scanf("%d",factors+i);
    }
    int limit;
    scanf("%d",&limit);
    long long *hamming=new long long[limit+1];
    hamming[0]=1;
    for(int i=1;i<=limit;i++){
        for(int j=0;j<FACTOR_NUM;j++){
            while(factors[j]*hamming[factorCoef[j]]<=hamming[i-1]){
                factorCoef[j]++;
            }
        }
        int minFactor=0;
        for(int j=1;j<FACTOR_NUM;j++){
            if(factors[j]*hamming[factorCoef[j]]<factors[minFactor]*hamming[factorCoef[minFactor]]){
                minFactor=j;
            }
        }
        hamming[i]=factors[minFactor]*hamming[factorCoef[minFactor]];
    }
    #ifdef DEBUG
    for(int i=0;i<=limit;i++){
        printf("%d:\t%d\n",i,hamming[i]);
    }
    #endif
    printf("%lld\n",hamming[limit]);
    delete[] hamming;
    return 0;
}
