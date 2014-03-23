import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private int height,width;
	private boolean[][] empty;
	private List<Step> path=new ArrayList<Step>();
	private class Step{
		private int lowerBound,upperBound;
		private char direction;
		public Step(int lowerBound,int upperBound,char direction){
			this.lowerBound=lowerBound;
			this.upperBound=upperBound;
			this.direction=direction;
		}
	}
	private class Coordinate{
		private int row,col;
		public Coordinate(int row,int col){
			this.row=row;
			this.col=col;
		}
		public Coordinate Advance(int distance,char direction){
			switch(direction){
				case 'U':return new Coordinate(row-distance,col);
				case 'D':return new Coordinate(row+distance,col);
				case 'R':return new Coordinate(row,col+distance);
				case 'L':return new Coordinate(row,col-distance);
				default:return null;
			}
		}
		public boolean Availiable(){
			return row>=0 && row<height && col>=0 && col<width && empty[row][col];
		}
	}
	public Main(){
		height=in.nextInt();
		width=in.nextInt();
		empty=new boolean[height][width];
		for(int row=0;row<height;row++)
			for(int col=0;col<width;col++)
				empty[row][col]=(in.nextInt()==0);
		while(true){
			int lowerBound=in.nextInt(),upperBound=in.nextInt();
			if(lowerBound==0 && upperBound==0)
				break;
			path.add(new Step(lowerBound,upperBound,in.next().charAt(0)));
		}
	}
	public void Output(){
		int count=0;
		for(int row=0;row<height;row++)
			for(int col=0;col<width;col++)
				if(PossibleToStartFrom(new Coordinate(row,col),0))
					count++;
		System.out.println(count);
	}
	private boolean PossibleToStartFrom(Coordinate coordinate,int stepBegin){
		if(stepBegin>=path.size())
			return true;
		Step step=path.get(stepBegin);
		for(int distance=step.lowerBound;distance<=step.upperBound;distance++)
			if(!Bumped(coordinate,distance,step.direction))
				if(PossibleToStartFrom(coordinate.Advance(distance,step.direction),stepBegin+1))
					return true;
		return false;
	}
	private boolean Bumped(Coordinate coordinate,int distance,char direction){
		while(distance>=0)
			if(!coordinate.Advance(distance--,direction).Availiable())
				return true;
		return false;
	}
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0)
			new Main().Output();
	}
}