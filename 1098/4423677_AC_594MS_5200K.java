import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static class Board implements Comparable<Board>{
		private enum Direction{
			UP,RIGHTUP,RIGHT,RIGHTDOWN,DOWN,LEFTDOWN,LEFT,LEFTUP
		}
		private static class Coordinate implements Comparable<Coordinate>{
			private int row,col;
			public Coordinate(int row,int col){
				this.row=row;
				this.col=col;
			}
			public Coordinate Move(Direction direction){
				switch(direction){
					case UP:		return new Coordinate(row+1,col);
					case RIGHTUP:	return new Coordinate(row+1,col+1);
					case RIGHT:		return new Coordinate(row,col+1);
					case RIGHTDOWN:	return new Coordinate(row-1,col+1);
					case DOWN:		return new Coordinate(row-1,col);
					case LEFTDOWN:	return new Coordinate(row-1,col-1);
					case LEFT:		return new Coordinate(row,col-1);
					case LEFTUP:	return new Coordinate(row+1,col-1);
					default:		return null;
				}
			}
			public Coordinate MoveTo(Coordinate destination){
				int nextRow=row,nextCol=col;
				if(destination.row!=row)
					nextRow+=Integer.signum(destination.row-row);
				if(destination.col!=col)
					nextCol+=Integer.signum(destination.col-col);
				return new Coordinate(nextRow,nextCol);
			}
			public boolean Legal(){
				return row>0 && row<=HEIGHT && col>0 && col<=WIDTH;
			}
			public int DistanceFrom(Coordinate another){
				return Math.abs(row-another.row)+Math.abs(col-another.col);
			}
			public Coordinate clone(){
				return new Coordinate(row,col);
			}
			public int compareTo(Coordinate another){
				if(row!=another.row)
					return Integer.signum(row-another.row);
				return Integer.signum(col-another.col);
			}
			public boolean equals(Coordinate another){
				return compareTo(another)==0;
			}
			public String toString(){
				return "("+row+","+col+")";
			}
		}
		private static final int HEIGHT=31,WIDTH=31,PLAYER_INITIAL_ROW=15,PLAYER_INITIAL_COL=15;
		private Set<Coordinate> robots=new TreeSet<Coordinate>(),debrises=new TreeSet<Coordinate>();
		private List<Coordinate> teleports=new ArrayList<Coordinate>();
		private Coordinate player=new Coordinate(PLAYER_INITIAL_ROW,PLAYER_INITIAL_COL);
		private int step;
		public Board(int step){
			this.step=step;
		}
		public void Input(int robotNum,int teleportNum){
			while(robotNum-->0)
				robots.add(new Coordinate(in.nextInt(),in.nextInt()));
			while(teleportNum-->0)
				teleports.add(new Coordinate(in.nextInt(),in.nextInt()));
		}
		public boolean Win(){
			return !Lose() && robots.isEmpty();
		}
		public boolean Lose(){
			return robots.contains(player) || debrises.contains(player);
		}
		public Board BestSolution(){
			Board bestSolution=RemainStationary();
			for(Direction direction:Direction.values()){
				if(CanPlayerMove(direction)){
					Board solution=PlayerMove(direction);
					if(solution.compareTo(bestSolution)>0)
						bestSolution=solution;
				}
			}
			if(bestSolution.Lose()){
				if(CanPlayerTeleport()){
					Board solution=PlayerTeleport();
					if(solution.compareTo(bestSolution)>0)
						bestSolution=solution;
				}
			}
			return bestSolution;
		}
		public boolean CanPlayerMove(Direction direction){
			Coordinate playerNext=player.Move(direction);
			if(!playerNext.Legal())
				return false;
			if(robots.contains(playerNext))
				return false;
			if(debrises.contains(playerNext)){
				Coordinate debrisNext=playerNext.Move(direction);
				if(!debrisNext.Legal())
					return false;
				if(debrises.contains(debrisNext))
					return false;
			}
			return true;
		}
		public Board PlayerMove(Direction direction){
			Board next=clone();
			next.player=player.Move(direction);
			if(next.debrises.contains(next.player)){
				next.debrises.remove(next.player);
				Coordinate debrisNext=next.player.Move(direction);
				if(next.robots.contains(debrisNext))
					next.robots.remove(debrisNext);
				next.debrises.add(debrisNext);
			}
			next.RobotsMove();
			return next;
		}
		public boolean CanPlayerTeleport(){
			for(Coordinate teleport:teleports)
				if(!player.equals(teleport) && !robots.contains(teleport) && !debrises.contains(teleport))
					return true;
			return false;
		}
		public Board PlayerTeleport(){
			for(Coordinate teleport:teleports)
				if(!player.equals(teleport) && !robots.contains(teleport) && !debrises.contains(teleport)){
					Board next=clone();
					next.player=teleport;
					next.teleports.remove(teleport);
					next.RobotsMove();
					if(!next.Lose()){
						System.out.println("Move "+next.step+": teleport to "+next.player);
						return next;
					}
				}
			return null;
		}
		public Board RemainStationary(){
			Board next=clone();
			next.RobotsMove();
			return next;
		}
		public void RobotsMove(){
			Set<Coordinate> robotsRemain=new TreeSet<Coordinate>();
			for(Coordinate robot:robots){
				Coordinate robotNext=robot.MoveTo(player);
				if(debrises.contains(robotNext))
					continue;
				if(robotsRemain.contains(robotNext)){
					robotsRemain.remove(robotNext);
					debrises.add(robotNext);
				}
				else
					robotsRemain.add(robotNext);
			}
			robots=robotsRemain;
		}
		public int MinimumDistance(){
			int distance=Integer.MAX_VALUE;
			for(Coordinate robot:robots)
				if(distance>robot.DistanceFrom(player))
					distance=robot.DistanceFrom(player);
			return distance;
		}
		public int compareTo(Board another){
			if(Lose())
				return -1;
			if(another.Lose())
				return 1;
			if(robots.size()!=another.robots.size())
				return Integer.signum(another.robots.size()-robots.size());
			if(MinimumDistance()!=another.MinimumDistance())
				return Integer.signum(MinimumDistance()-another.MinimumDistance());
			return another.player.compareTo(player);
		}
		public Board clone(){
			Board clone=new Board(step+1);
			clone.player=new Coordinate(player.row,player.col);
			clone.robots.addAll(robots);
			clone.debrises.addAll(debrises);
			clone.teleports.addAll(teleports);
			return clone;
		}
	}
	Board current;
	public Main(int robotNum,int teleportNum){
		current=new Board(0);
		current.Input(robotNum,teleportNum);
	}
	public void Output(){
		while(!current.Win() && !current.Lose())
			current=current.BestSolution();
		if(current.Win()){
			System.out.println("Won game after making "+current.step+" moves.");
			System.out.println("Final position: "+current.player);
			System.out.println("Number of cells with debris: "+current.debrises.size()); 
		}
		else{
			System.out.println("Lost game after making "+current.step+" moves.");
			System.out.println("Final position: "+current.player);
			System.out.println("Number of cells with debris: "+current.debrises.size()); 
			System.out.println("Number of robots remaining: "+current.robots.size());
		}
		System.out.println();
	}
	public static void main(String[] args){
		int caseID=1;
		while(true){
			int robotNum=in.nextInt(),teleportNum=in.nextInt();
			if(robotNum==0 && teleportNum==0)
				break;
			Main game=new Main(robotNum,teleportNum);
			System.out.println("Case "+(caseID++)+":");
			game.Output();
		}
	}
}