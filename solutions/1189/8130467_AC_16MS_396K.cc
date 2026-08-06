#include<cstdio>
#include<algorithm>

const int SIZE_MAX=60;

long long gcd(long long greater,long long less){
    if(greater<less){
        std::swap(greater,less);
    }
    if(greater%less==0){
        return less;
    }
    return gcd(greater%less,less);
}

int main(){
    int size,pos;
    scanf("%d%d",&size,&pos);
    char map[SIZE_MAX][SIZE_MAX];
    for(int row=0;row<size;row++){
        for(int col=0;col<=row;col++){
            do{
                scanf("%c",map[row]+col);
            }while(map[row][col]==' ' || map[row][col]=='\n');
        }
    }
    long long count[SIZE_MAX+1][SIZE_MAX+1];
    for(int row=0;row<=size;row++){
        for(int col=0;col<=row;col++){
            count[row][col]=0;
        }
    }
    count[0][0]=1;
    count[0][0]<<=size;
    for(int row=0;row<size;row++){
        for(int col=0;col<=row;col++){
            if(map[row][col]=='*' || row==size-1){
                count[row+1][col]+=(count[row][col]>>1);
                count[row+1][col+1]+=(count[row][col]>>1);
            }
            else{
                count[row+2][col+1]+=count[row][col];
            }
        }
    }
    if(count[size][pos]==0){
        printf("0\n");
    }
    else{
        long long numerator=count[size][pos],denominator=count[0][0];
        long long divisor=gcd(denominator,numerator);
        printf("%lld/%lld\n",numerator/divisor,denominator/divisor);
    }
    return 0;
}