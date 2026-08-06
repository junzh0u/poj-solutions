#include<cstdio>
#include<algorithm>
using namespace std;

int main(){
    int size;
    scanf("%d",&size);
    int *lastColumn=new int[size],*firstColumn=new int[size];
    for(int i=0;i<size;i++){
        scanf("%d",lastColumn+i);
        firstColumn[i]=lastColumn[i];
    }
    sort(firstColumn,firstColumn+size);
    int *next=new int[size],ptrZero=0,ptrOne=0;
    while(ptrZero<size && lastColumn[ptrZero]!=0)
        ptrZero++;
    while(ptrOne<size && lastColumn[ptrOne]!=1)
        ptrOne++;
    for(int i=0;i<size;i++){
        if(firstColumn[i]==0){
            next[i]=ptrZero++;
            while(ptrZero<size && lastColumn[ptrZero]!=0)
                ptrZero++;
        }
        else{
            next[i]=ptrOne++;
            while(ptrOne<size && lastColumn[ptrOne]!=1)
                ptrOne++;
        }
    }
    int current=0;
    for(int i=0;i<size;i++){
        current=next[current];
        printf("%d ",lastColumn[current]);
    }
    printf("\n");
    delete[] lastColumn;
    delete[] firstColumn;
    return 0;
}