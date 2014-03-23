import java.util.*;

public class Main {
	private static final Scanner in=new Scanner(System.in);
	private int width,height;
	private String path;
	private class Grid{
		public boolean inPath=false;
		public static final int START=0,END=1;
		public int[] distances={Integer.MAX_VALUE,Integer.MAX_VALUE};
		public int row,col;
		public Grid(){
			col=in.nextInt();
			row=in.nextInt();
		}
		public Grid(int row,int col){
			this.row=row;
			this.col=col;
		}
	}
	private Grid[][] maze;
	private Grid start,end;
	private class Wall{
		private int rowSum,colSum;
		private Grid left,right;
		public Wall(Grid left,Grid right){
			this.left=left;
			this.right=right;
			rowSum=left.row+right.row;
			colSum=left.col+right.col;
		}
		public Grid Left(){
			return maze[left.row][left.col];
		}
		public Grid Right(){
			return maze[right.row][right.col];
		}
		public int hashCode(){
			return rowSum*colSum;
		}
		public boolean equals(Object o){
			if(!(o instanceof Wall))
				return false;
			Wall w=(Wall)o;
			return rowSum==w.rowSum && colSum==w.colSum;
		}
	}
	private Set<Wall> walls=new HashSet<Wall>();
	boolean correct;
	public Main(){
		width=in.nextInt();
		height=in.nextInt();
		maze=new Grid[height][width];
		for(int row=0;row<height;row++)
			for(int col=0;col<width;col++)
				maze[row][col]=new Grid(row,col);
		start=maze[0][0];
		path=in.next();
		int wallNum=in.nextInt();
		while(wallNum-->0){
			Grid left=new Grid();
			Grid right=new Grid();
			walls.add(new Wall(left,right));
		}
		correct=IsCorrect();
	}
	private boolean IsCorrect(){
		if(!TravelPath())
			return false;
		SearchFromStart();
		SearchFromEnd();
		if(!UniquePath())
			return false;
		if(UnnecessaryWall())
			return false;
		return true;
	}
	private boolean TravelPath(){
		Grid current=start;
		current.inPath=true;
		for(int step=0;step<path.length();step++){
			int row=current.row,col=current.col;
			switch(path.charAt(step)){
			case 'U':
				row++;break;
			case 'D':
				row--;break;
			case 'L':
				col--;break;
			case 'R':
				col++;break;
			}
			Grid next=maze[row][col];
			if(walls.contains(new Wall(current,next)))
				return false;
			next.inPath=true;
			current=next;
		}
		end=current;
		return true;
	}
	private void SearchFromStart(){
		SearchAssistant(start,0,Grid.START);
	}
	private void SearchFromEnd(){
		SearchAssistant(end,0,Grid.END);
	}
	private void SearchAssistant(Grid current,int distance,int type){
		if(current.distances[type]<=distance)
			return;
		current.distances[type]=distance;
		final int[][] STEP={{1,0},{-1,0},{0,-1},{0,1}};
		for(int i=0;i<4;i++){
			int rowNext=current.row+STEP[i][0],colNext=current.col+STEP[i][1];
			if(rowNext<0 || rowNext>=height || colNext<0 || colNext>=width)
				continue;
			Grid next=maze[rowNext][colNext];
			if(!walls.contains(new Wall(current,next)))
				SearchAssistant(next,distance+1,type);
		}
	}
	private boolean UniquePath(){
		for(int row=0;row<height;row++)
			for(int col=0;col<width;col++){
				Grid grid=maze[row][col];
				if(!grid.inPath && grid.distances[0]+grid.distances[1]<=path.length())
					return false;
			}
		return true;
	}
	private boolean UnnecessaryWall(){
		for(Iterator<Wall> iter=walls.iterator();iter.hasNext();){
			Wall wall=iter.next();
			Grid left=wall.Left(),right=wall.Right();
			if(	left.distances[0]+right.distances[1]+1>path.length() &&
				left.distances[1]+right.distances[0]+1>path.length())
				return true;
		}
		return false;
	}
	public void Output(){
		System.out.println(correct?"CORRECT":"INCORRECT");
	}
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0){
			Main problem=new Main();
			problem.Output();
		}
	}
}