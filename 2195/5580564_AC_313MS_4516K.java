import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);
	public static void main(String[] args){
		while(true){
			int height = in.nextInt(),width = in.nextInt();
			if(height == 0 && width == 0)
				break;
			new Main(height,width).output();
		}
	}
	public class Unit{
		private int row,col,topmark;
		private boolean visited;
		public Unit(int row,int col){
			this.row = row;
			this.col = col;
			setTopmark(0);
			setVisited(false);
		}
		public int DistanceFrom(Unit another){
			return Math.abs(row - another.row) + Math.abs(col - another.col);
		}
		public int getTopmark(){
			return topmark;
		}
		public void setTopmark(int topmark){
			this.topmark = topmark;
		}
		public boolean isVisited(){
			return visited;
		}
		public void setVisited(boolean visited){
			this.visited = visited;
		}
	}
	public class Man extends Unit{
		public Man(int row,int col){
			super(row,col);
		}
	}
	public class House extends Unit{
		private Man match;
		private int slack;
		public House(int row,int col){
			super(row,col);
			setMatch(null);
			setSlack(0);
		}
		public Man getMatch(){
			return match;
		}
		public void setMatch(Man match){
			this.match = match;
		}
		public int getSlack(){
			return slack;
		}
		public void setSlack(int slack){
			this.slack = slack;
		}
	}
	private ArrayList<Man> men;
	private ArrayList<House> houses;
	private HashMap<Unit,HashMap<Unit,Integer>> edges;
	public Main(int height,int width){
		men = new ArrayList<Man>();
		houses = new ArrayList<House>();
		for(int row = 0;row < height;row++){
			char line[] = in.next().toCharArray();
			for(int col = 0;col < width;col++)
				if(line[col] == 'm')
					men.add(new Man(row,col));
				else if(line[col] == 'H')
					houses.add(new House(row,col));
		}
		edges = new HashMap<Unit,HashMap<Unit,Integer>>();
		for(Man man:men){
			edges.put(man,new HashMap<Unit,Integer>());
			for(House house:houses)
				edges.get(man).put(house,man.DistanceFrom(house));
		}
		kuhnMunkras();
	}
	private void kuhnMunkras(){
		for(Man man:men)
			for(House house:houses)
				edges.get(man).put(house,-edges.get(man).get(house));
		for(Man man:men){
			man.setTopmark(Integer.MIN_VALUE);
			for(House house:houses)
				man.setTopmark(Math.max(man.getTopmark(),edges.get(man).get(house)));
		}
		for(House house:houses){
			house.setTopmark(0);
			house.setMatch(null);
		}
		for(Man man:men)
			while(true){
				for(Man manTemp:men)
					manTemp.setVisited(false);
				for(House house:houses){
					house.setVisited(false);
					house.setSlack(Integer.MAX_VALUE);
				}
				if(FindAndFlipAugumentPath(man))
					break;
				int slack = Integer.MAX_VALUE;
				for(House house:houses)
					if(!house.isVisited())
						slack = Math.min(slack,house.getSlack());
				for(Man manTemp:men)
					if(manTemp.isVisited())
						manTemp.setTopmark(manTemp.getTopmark() - slack);
				for(House house:houses)
					if(house.isVisited())
						house.setTopmark(house.getTopmark() + slack);
			}
		for(Man man:men)
			for(Unit house:houses)
				edges.get(man).put(house,-edges.get(man).get(house));
	}
	private boolean FindAndFlipAugumentPath(Man manBegin){
		manBegin.setVisited(true);
		for(House house:houses)
			if(!house.isVisited())
				if(manBegin.getTopmark() + house.getTopmark() == edges.get(manBegin).get(house)){
					house.setVisited(true);
					if(house.getMatch() == null || FindAndFlipAugumentPath(house.getMatch())){
						house.setMatch(manBegin);
						return true;
					}
				}
				else
					house.setSlack(Math.min(house.getSlack(),manBegin.getTopmark() + house.getTopmark() - edges.get(manBegin).get(house)));
		return false;
	}
	public void output(){
		int weightSum = 0;
		for(House house:houses)
			weightSum += edges.get(house.getMatch()).get(house);
		System.out.println(weightSum);
	}
}