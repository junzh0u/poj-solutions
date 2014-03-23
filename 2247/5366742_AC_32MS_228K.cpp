#include<cstdio>


int main(){
    const int LIMIT=5842,FACTOR_NUM=4,FACTORS[FACTOR_NUM]={2,3,5,7};
    int factorCoef[FACTOR_NUM]={0,0,0,0},humble[LIMIT]={1,2,3,4,5,6,7,8,9,10,12,14,15,16,18,20,21,24,25,27};
    for(int i=20;i<LIMIT;i++){
        for(int j=0;j<FACTOR_NUM;j++){
            while(FACTORS[j]*humble[factorCoef[j]]<=humble[i-1]){
                factorCoef[j]++;
            }
        }
        int minFactor=0;
        for(int j=1;j<FACTOR_NUM;j++){
            if(FACTORS[j]*humble[factorCoef[j]]<FACTORS[minFactor]*humble[factorCoef[minFactor]]){
                minFactor=j;
            }
        }
        humble[i]=FACTORS[minFactor]*humble[factorCoef[minFactor]];
    }
    int n;
    while(true){
        scanf("%d",&n);
        if(n==0){
            break;
        }
        printf("The %d",n);
        if(n%10==1 && n%100!=11){
            printf("st");
        }
        else if(n%10==2 && n%100!=12){
            printf("nd");
        }
        else if(n%10==3 && n%100!=13){
            printf("rd");
        }
        else{
            printf("th");
        }
        printf(" humble number is %d.\n",humble[n-1]);
    }
    return 0;
}