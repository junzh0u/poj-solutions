#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

const int CAKE_SIZE_MAX=25;

class Cake{
private:
    int cakeSize;
    vector<int> pieceSizes;
    bool covered[2*CAKE_SIZE_MAX][4*CAKE_SIZE_MAX];
public:
    void Input(){
        int pieceTypeNum;
        cin>>cakeSize>>pieceTypeNum;
        pieceSizes.clear();
        while(pieceTypeNum-->0){
            int pieceSize;
            cin>>pieceSize;
            if(pieceSize<=cakeSize)
                pieceSizes.push_back(pieceSize);
        }
        sort(pieceSizes.begin(),pieceSizes.end());
        for(vector<int>::iterator iter=pieceSizes.begin();iter!=pieceSizes.end();iter++){
            for(vector<int>::iterator before=pieceSizes.begin();before!=iter;before++){
                if(*iter%*before==0){
                    pieceSizes.erase(iter);
                    iter--;
                    break;
                }
            }
        }
    }
    bool CanDivide(){
        for(unsigned i=0;i<pieceSizes.size();i++)
            if(cakeSize%pieceSizes[i]==0)
                return true;
        for(int i=0;i<2*cakeSize;i++)
            for(int j=0;j<4*cakeSize;j++)
                covered[i][j]=false;
        for(int row=0;row<cakeSize;row++)
            for(int col=2*(cakeSize+row)+1;col<4*cakeSize;col++)
                covered[row][col]=true;
        for(int row=cakeSize;row<2*cakeSize;row++)
            for(int col=0;col<=2*(row-cakeSize);col++)
                covered[row][col]=true;
        return Search(0,0);
    }
private:
    bool Search(int row,int col){
        if(row>=2*cakeSize)
            return true;
        if(col>=4*cakeSize)
            return Search(row+1,0);
        if(covered[row][col])
            return Search(row,col+1);
        int sizeMax=SizeAvailiableMax(row,col);
        for(unsigned i=0;i<pieceSizes.size();i++){
            if(pieceSizes[i]>sizeMax)
                break;
            Fill(row,col,pieceSizes[i],true);
            if(Search(row,col+1))
                return true;
            Fill(row,col,pieceSizes[i],false);
        }
        return false;
    }
    int SizeAvailiableMax(int row,int col){
        for(int size=1;size<cakeSize;size++)
            if(!SizeAvailiable(row,col,size))
                return size-1;
        return cakeSize;
    }
    bool SizeAvailiable(int row,int col,int size){
        if(col%2==0){
            for(int fillRow=row;fillRow<row+size;fillRow++)
                for(int fillCol=col;fillCol<=col+(fillRow-row)*2;fillCol++)
                    if(fillRow>=2*cakeSize || fillCol>=4*cakeSize || covered[fillRow][fillCol])
                        return false;
        }
        else{
            for(int fillRow=row;fillRow<row+size;fillRow++)
                for(int fillCol=col+(fillRow-row)*2;fillCol<=col+2*(size-1);fillCol++)
                    if(fillRow>=2*cakeSize || fillCol>=4*cakeSize || covered[fillRow][fillCol])
                        return false;
        }
        return true;
    }
    void Fill(int row,int col,int size,bool value){
        if(col%2==0){
            for(int fillRow=row;fillRow<row+size;fillRow++)
                for(int fillCol=col;fillCol<=col+(fillRow-row)*2;fillCol++)
                    covered[fillRow][fillCol]=value;
        }
        else{
            for(int fillRow=row;fillRow<row+size;fillRow++)
                for(int fillCol=col+(fillRow-row)*2;fillCol<=col+2*(size-1);fillCol++)
                    covered[fillRow][fillCol]=value;
        }
    }
};

int main(){
    int caseNum;
    cin>>caseNum;
    Cake testCase;
    while(caseNum-->0){
        testCase.Input();
        cout<<(testCase.CanDivide()?"YES":"NO")<<endl;
    }
}