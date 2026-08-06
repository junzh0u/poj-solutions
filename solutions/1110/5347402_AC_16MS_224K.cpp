#include<cstdio>
using namespace std;

#define DEBUG

const char EMPTY='.',INK='o',POLYMER='#';

bool TryOnePixal(char*** symbols,int ID,int num,int height,int width){
    for(int row=0;row<height;row++){
        for(int col=0;col<width;col++){
            if(symbols[ID][row][col]!=EMPTY){
                bool found=true;
                for(int i=0;i<num;i++){
                    if(i!=ID && symbols[i][row][col]!=EMPTY){
                        found=false;
                        break;
                    }
                }
                if(found){
                    symbols[ID][row][col]=POLYMER;
                    return true;
                }
            }
        }
    }
    return false;
}

bool TryTwoPixal(char*** symbols,int ID,int num,int height,int width){
    for(int positionA=0;positionA<height*width-1;positionA++){
        int rowA=positionA/width,colA=positionA%width;
        if(symbols[ID][rowA][colA]!=EMPTY){
            for(int positionB=positionA+1;positionB<height*width;positionB++){
                int rowB=positionB/width,colB=positionB%width;
                if(symbols[ID][rowB][colB]!=EMPTY){
                    bool found=true;
                    for(int i=0;i<num;i++){
                        if(i!=ID && symbols[i][rowA][colA]!=EMPTY && symbols[i][rowB][colB]!=EMPTY){
                            found=false;
                            break;
                        }
                    }
                    if(found){
                        symbols[ID][rowA][colA]=symbols[ID][rowB][colB]=POLYMER;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main(){
    int caseID=1;
    while(true){
        int num,height,width;
        scanf("%d%d%d",&num,&height,&width);
        if(num==0 && height==0 && width==0){
            break;
        }
        char ***symbols=new char**[num];
        for(int i=0;i<num;i++){
            symbols[i]=new char*[height];
            for(int row=0;row<height;row++){
                symbols[i][row]=new char[width+1];
            }
        }
        for(int row=0;row<height;row++){
            for(int i=0;i<num;i++){
                scanf("%s",symbols[i][row]);
            }
        }
        printf("Test %d\n",caseID++);
        bool possible=true;
        for(int i=0;i<num;i++){
            if(!TryOnePixal(symbols,i,num,height,width)){
                if(!TryTwoPixal(symbols,i,num,height,width)){
                    possible=false;
                    break;
                }
            }
        }
        if(possible){
            for(int row=0;row<height;row++){
                for(int i=0;i<num;i++){
                    printf("%s ",symbols[i][row]);
                }
                printf("\n");
            }
        }
        else{
            printf("impossible\n");
        }
        for(int i=0;i<num;i++){
            for(int row=0;row<height;row++){
                delete[] symbols[i][row];
            }
            delete[] symbols[i];
        }
        delete[] symbols;
    }
    return 0;
}