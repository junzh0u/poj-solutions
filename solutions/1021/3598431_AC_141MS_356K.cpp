#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
using namespace std;

template<class T>
bool Equal(set<T> left,set<T> right){
	if(left.size()!=right.size())
		return false;
	for(set<T>::iterator iter=left.begin();iter!=left.end();iter++)
		if(right.find(*iter)==right.end())
			return false;
	return true;
}

enum Direction{
	Dir_Begin=0,
	Dir_Up=0,
	Dir_Right,
	Dir_Down,
	Dir_Left,
	Dir_End
};

class Grid{
public:
	Grid(int row=0,int col=0):row(row),col(col){
	}
	bool operator==(const Grid another) const{
		return row==another.row && col==another.col;
	}
	bool operator<(const Grid right) const{
		return row<right.row || (row==right.row && col<right.col);
	}
	Grid Next(Direction dir) const{
		static const int STEP[Dir_End][2]={-1,0,0,1,1,0,0,-1};
		return Grid(row+STEP[dir][0],col+STEP[dir][1]);
	}
	int row,col;
};

class Block{
public:
	Block(set<Grid> grids){
		Move(grids);
		height=width=0;
		for(set<Grid>::const_iterator iter=grids.begin();iter!=grids.end();iter++){
			if(height<iter->row)
				height=iter->row;
			if(width<iter->col)
				width=iter->col;
		}
		for(int i=0;i<4;i++){
			morphs[i]=grids;
			Whirl(grids);
		}
		Reverse(grids);
		for(int i=4;i<8;i++){
			morphs[i]=grids;
			Whirl(grids);
		}
	}
	bool Similar(Block another){
		for(int i=0;i<8;i++)
			if(Equal(morphs[i],another.morphs[0]))
				return true;
		return false;
	}
private:
	void Move(set<Grid>& grids){
		int rowMin=INT_MAX,colMin=INT_MAX;
		for(set<Grid>::const_iterator iter=grids.begin();iter!=grids.end();iter++){
			if(rowMin>iter->row)
				rowMin=iter->row;
			if(colMin>iter->col)
				colMin=iter->col;
		}
		for(set<Grid>::iterator iter=grids.begin();iter!=grids.end();iter++){
			iter->row-=rowMin;
			iter->col-=colMin;
		}
	}
	void Whirl(set<Grid>& grids){
		for(set<Grid>::iterator iter=grids.begin();iter!=grids.end();iter++)
			*iter=Grid(width-iter->col,iter->row);
		swap(height,width);
	}
	void Reverse(set<Grid>& grids){
		for(set<Grid>::iterator iter=grids.begin();iter!=grids.end();iter++)
			*iter=Grid(iter->row,width-iter->col);
	}
	int height,width;
	set<Grid> morphs[8];
};

class Board{
public:
	Board(int height,int width):height(height),width(width){
	}
	void ReadGrid(int gridNum){
		set<Grid> board;
		while(gridNum-->0){
			Grid grid;
			cin>>grid.row>>grid.col;
			board.insert(grid);
		}
		SeparateBlocks(board);
	}
	void SeparateBlocks(set<Grid> board){
		map<Grid,bool> visited;
		for(set<Grid>::const_iterator iter=board.begin();iter!=board.end();iter++)
			if(!visited[*iter])
				blocks.push_back(Block(CreateBlock(iter,board,visited)));
	}
	set<Grid> CreateBlock(set<Grid>::const_iterator current,const set<Grid>& board,map<Grid,bool>& visited){
		if(visited[*current])
			return set<Grid>();
		visited[*current]=true;
		set<Grid> block;
		block.insert(*current);
		for(Direction dir=Dir_Begin;dir!=Dir_End;dir=Direction(dir+1)){
			Grid next=current->Next(dir);
			if(visited[next])
				continue;
			set<Grid>::const_iterator nextIter=board.find(next);
			if(nextIter!=board.end()){
				set<Grid> temp=CreateBlock(nextIter,board,visited);
				block.insert(temp.begin(),temp.end());
			}
		}
		return block;
	}
	bool Equivalent(Board another){
		if(blocks.size()!=another.blocks.size())
			return false;
		vector<bool> used(blocks.size(),false);
		for(unsigned i=0;i<blocks.size();i++){
			bool found=false;
			for(unsigned j=0;j<blocks.size();j++)
				if(!used[j] && blocks[i].Similar(another.blocks[j])){
					used[j]=found=true;
					break;
				}
			if(!found)
				return false;
		}
		return true;
	}
private:
	int height,width;
	vector<Block> blocks;
};

int main(){
	int caseNum;
	cin>>caseNum;
	while(caseNum-->0){
		int height,width,gridNum;
		cin>>height>>width>>gridNum;
		Board A(height,width),B(height,width);
		A.ReadGrid(gridNum);
		B.ReadGrid(gridNum);
		cout<<(A.Equivalent(B)?"YES":"NO")<<endl;
	}
	return 0;
}