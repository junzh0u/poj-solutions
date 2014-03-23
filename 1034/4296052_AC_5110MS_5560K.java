import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int DOG_SPEED=2;
	private List<Point> interestingPlaces=new ArrayList<Point>();
	private List<Section> route=new ArrayList<Section>();
	Node source=new Node(),destination=new Node();
	private class Node{
		protected Map<Node,Boolean> nexts=new HashMap<Node,Boolean>();
		public Node(){
		}
		public void AddNext(Node next,boolean value){
			nexts.put(next,value);
		}
	}
	private class Point extends Node{
		private int x,y;
		public Point(){
			x=in.nextInt();
			y=in.nextInt();
		}
		public double DistanceFrom(Point next){
			return Math.sqrt((x-next.x)*(x-next.x)+(y-next.y)*(y-next.y));
		}
		public String toString(){
			return x+" "+y;
		}
	}
	private class Section extends Node{
		private Point endA,endB;
		private double length;
		private Point interestingPlaceVisited=null;
		public Section(Point endA,Point endB){
			this.endA=endA;
			this.endB=endB;
			length=endA.DistanceFrom(endB);
		}
		public boolean CanVisit(Point interestingPlace){
			return interestingPlace.DistanceFrom(endA)+interestingPlace.DistanceFrom(endB)<=DOG_SPEED*length;
		}
		public void ChooseInterestingPlace(){
			for(Node interestingPlace:nexts.keySet()){
				if(interestingPlace!=source && !nexts.get(interestingPlace)){
					interestingPlaceVisited=(Point)interestingPlace;
					break;
				}
			}
		}
	}
	public Main(){
		Input();
		Analyze();
	}
	private void Input(){
		int routeVertexNum=in.nextInt();
		int interestingPlaceNum=in.nextInt();
		Point prev=null;
		while(routeVertexNum-->0){
			Point current=new Point();
			if(prev!=null){
				Section section=new Section(prev,current);
				source.AddNext(section,true);
				section.AddNext(source,false);
				route.add(section);
			}
			prev=current;
		}
		while(interestingPlaceNum-->0){
			Point interestingPlace=new Point();
			interestingPlace.AddNext(destination,true);
			destination.AddNext(interestingPlace,false);
			for(Section section:route){
				if(section.CanVisit(interestingPlace)){
					section.AddNext(interestingPlace,true);
					interestingPlace.AddNext(section,false);
				}
			}
			interestingPlaces.add(interestingPlace);
		}
	}
	private void Analyze(){
		while(true){
			Stack<Node> augmentingPath=FindAugmentingPath();
			if(augmentingPath==null)
				break;
			AddPath(augmentingPath);
		}
		for(Section section:route)
			section.ChooseInterestingPlace();
	}
	private Stack<Node> FindAugmentingPath(){
		Queue<Node> BFS=new LinkedList<Node>();
		Map<Node,Node> father=new HashMap<Node,Node>();
		BFS.add(source);
		father.put(source,null);
		while(!BFS.isEmpty()){
			Node current=BFS.poll();
			if(current==destination){
				Stack<Node> augmentingPath=new Stack<Node>();
				while(current!=null){
					augmentingPath.push(current);
					current=father.get(current);
				}
				return augmentingPath;
			}
			for(Node next:current.nexts.keySet()){
				if(current.nexts.get(next) && !father.containsKey(next)){
					BFS.add(next);
					father.put(next,current);
				}
			}
		}
		return null;
	}
	private void AddPath(Stack<Node> path){
		Node current=path.pop();
		while(!path.empty()){
			Node next=path.pop();
			current.nexts.put(next,false);
			next.nexts.put(current,true);
			current=next;
		}
	}
	public void Output(){
		int vertexNum=route.size()+1;
		for(Section section:route)
			if(section.interestingPlaceVisited!=null)
				vertexNum++;
		System.out.println(vertexNum);
		System.out.print(route.get(0).endA);
		for(Section section:route){
			if(section.interestingPlaceVisited!=null)
				System.out.print(" "+section.interestingPlaceVisited+"");
			System.out.print(" "+section.endB);
		}
		System.out.println();
	}
	public static void main(String[] args){
		Main walk=new Main();
		walk.Output();
	}
}
