#include<cstdio>
using namespace std;

//#define DEBUG

void Paint(char** grid,const int row,const int col,const int height,const int width,const char from,const char to){
    if(row<0 || row>=height)
        return;
    if(col<0 || col>=width)
        return;
    if(grid[row][col]!=from)
        return;
    grid[row][col]=to;
    Paint(grid,row-1,col-1,height,width,from,to);
    Paint(grid,row-1,col,height,width,from,to);
    Paint(grid,row-1,col+1,height,width,from,to);
    Paint(grid,row,col-1,height,width,from,to);
    Paint(grid,row,col+1,height,width,from,to);
    Paint(grid,row+1,col-1,height,width,from,to);
    Paint(grid,row+1,col,height,width,from,to);
    Paint(grid,row+1,col+1,height,width,from,to);
}

int main(){
    while(true){
        int height,width,clickRow,clickCol;
        scanf("%d%d%d%d",&height,&width,&clickRow,&clickCol);
        if(height==0 && width==0 && clickRow==0 && clickCol==0)
            break;
        char **grid;
        grid=new char*[height];
        for(int row=0;row<height;row++){
            grid[row]=new char[width+1];
            scanf("%s",grid[row]);
        }
        Paint(grid,--clickRow,--clickCol,height,width,'X','O');
        #ifdef DEBUG
        for(int row=0;row<height;row++)
            printf("%s\n",grid[row]);
        #endif
        int perimeter=0;
        for(int row=0;row<height;row++){
            for(int col=0;col<width;col++){
                if(grid[row][col]=='O'){
                    if(row-1<0 || grid[row-1][col]!='O'){
                        perimeter++;
                    }
                    if(row+1>=height || grid[row+1][col]!='O'){
                        perimeter++;
                    }
                    if(col-1<0 || grid[row][col-1]!='O'){
                        perimeter++;
                    }
                    if(col+1>=width || grid[row][col+1]!='O'){
                        perimeter++;
                    }
                }
            }
        }
        printf("%d\n",perimeter);
    }
    return 0;
}