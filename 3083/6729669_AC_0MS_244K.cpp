#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

enum TurnType{
	CLOCKWISE=1,ANTICLOCKWISE=3
};

class Direction{
public:
	Direction(int value=0){
		this->value=value;
	}
	Direction turn(TurnType turnType){
		return Direction((value+turnType)%4);
	}
	int rowStep() const{
		return ROW_STEP[value];
	}
	int colStep() const{
		return COL_STEP[value];
	}
	bool operator==(const Direction &another){
		return value==another.value;
	}
	bool operator!=(const Direction &another){
		return !(*this==another);
	}
	static const Direction UP,RIGHT,DOWN,LEFT;
private:
	static const int ROW_STEP[4],COL_STEP[4];
	int value;
};
const Direction Direction::UP(0);
const Direction Direction::RIGHT(1);
const Direction Direction::DOWN(2);
const Direction Direction::LEFT(3);
const int Direction::ROW_STEP[4]={-1,0,1,0};
const int Direction::COL_STEP[4]={0,1,0,-1};

class Coordinate{
public:
	Coordinate(int row=0,int col=0){
		this->row=row;
		this->col=col;
	}
	Coordinate next(const Direction &direction){
		return Coordinate(row+direction.rowStep(),col+direction.colStep());
	}
	int getRow() const{
		return row;
	}
	int getCol() const{
		return col;
	}
	bool operator==(const Coordinate &another){
		return row==another.row && col==another.col;
	}
	bool operator!=(const Coordinate &another){
		return !(*this==another);
	}
private:
	int row,col;
};

class Maze{
public:
	Maze(int height,int width){
		this->height=height;
		this->width=width;
		grids=new char*[height];
		for(int row=0;row<height;row++)
			grids[row]=new char[width+1];
		for(int row=0;row<height;row++){
			scanf("%s",grids[row]);
			for(int col=0;col<width;col++){
				if(grids[row][col]==START)
					start=Coordinate(row,col);
				else if(grids[row][col]==EXIT)
					exit=Coordinate(row,col);
			}
		}
		if(start.getRow()==0)
			directionBegin=Direction::DOWN;
		else if(start.getRow()==height-1)
			directionBegin=Direction::UP;
		else if(start.getCol()==0)
			directionBegin=Direction::RIGHT;
		else if(start.getCol()==width-1)
			directionBegin=Direction::LEFT;
	}
	~Maze(){
		for(int row=0;row<height;row++)
			delete[] grids[row];
		delete[] grids;
	}
	bool avaliable(const Coordinate &position){
		return 	position.getRow()>=0 && position.getRow()<height && position.getCol()>=0 && position.getCol()<width &&
				(grids[position.getRow()][position.getCol()]==EMPTY || grids[position.getRow()][position.getCol()]==EXIT);
	}
	int leftPath(){
		return followSidePath(ANTICLOCKWISE,CLOCKWISE);
	}
	int rightPath(){
		return followSidePath(CLOCKWISE,ANTICLOCKWISE);
	}
	int shortestPath(){
		Coordinate *BFSQueue=new Coordinate[height*width];
		bool **visited=new bool*[height];
		for(int row=0;row<height;row++)
			visited[row]=new bool[width];
		int **step=new int*[height];
		for(int row=0;row<height;row++)
			step[row]=new int[width];

		for(int row=0;row<height;row++)
			for(int col=0;col<width;col++)
				visited[row][col]=false;
		int head=0,tail=0;
		BFSQueue[tail++]=start;
		visited[start.getRow()][start.getCol()]=true;
		step[start.getRow()][start.getCol()]=1;
		while(head<tail){
			Coordinate position=BFSQueue[head];
			head++;
			if(position==exit)
				return step[position.getRow()][position.getCol()];
			Direction direction=Direction::UP;
			do{
				Coordinate positionNext=position.next(direction);
				if(avaliable(positionNext) && !visited[positionNext.getRow()][positionNext.getCol()]){
					BFSQueue[tail++]=positionNext;
					visited[positionNext.getRow()][positionNext.getCol()]=true;
					step[positionNext.getRow()][positionNext.getCol()]=step[position.getRow()][position.getCol()]+1;
				}
				direction=direction.turn(CLOCKWISE);
			}while(direction!=Direction::UP);
		}

		for(int row=0;row<height;row++)
			delete[] step[row];
		delete[] step;
		for(int row=0;row<height;row++)
			delete[] visited[row];
		delete[] visited;
		delete[] BFSQueue;
		return 0;
	}
private:
	int followSidePath(TurnType aim,TurnType antiaim){
		Coordinate position=start;
		Direction direction=directionBegin;
		int stepCount=1;
		while(position!=exit){
			for(Direction directionNext=direction.turn(aim);;directionNext=directionNext.turn(antiaim)){
				Coordinate positionNext=position.next(directionNext);
				if(avaliable(positionNext)){
					position=positionNext;
					direction=directionNext;
					stepCount++;
					break;
				}
			}
		}
		return stepCount;
	}
	static const char START='S',EXIT='E',WALL='#',EMPTY='.';
	int height,width;
	char **grids;
	Coordinate start,exit;
	Direction directionBegin;
};

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int height,width;
		scanf("%d%d",&width,&height);
		Maze maze(height,width);
		printf("%d %d %d\n",maze.leftPath(),maze.rightPath(),maze.shortestPath());
	}
	return 0;
}