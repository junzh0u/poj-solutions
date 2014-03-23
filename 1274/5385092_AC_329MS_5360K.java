import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Scanner;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private class Point{
		public List<Point> nexts;
		public Point(){
			nexts=new ArrayList<Point>();
		}
	}
	Point source,destination,cows[],stalls[];
	public Main(int cowNum,int stallNum){
		source=new Point();
		destination=new Point();
		cows=new Point[cowNum];
		stalls=new Point[stallNum];
		for(int i=0;i<stallNum;i++){
			stalls[i]=new Point();
			stalls[i].nexts.add(destination);
		}
		for(int i=0;i<cowNum;i++){
			cows[i]=new Point();
			source.nexts.add(cows[i]);
			int nextNum=in.nextInt();
			while(nextNum-->0)
				cows[i].nexts.add(stalls[in.nextInt()-1]);
		}
	}
	private int MaxFlow(){
		int count=0;
		while(true){
			List<Point> augumentPath=AugumentPath();
			if(augumentPath==null)
				break;
			RemovePath(augumentPath);
			count++;
		}
		return count;
	}
	private List<Point> AugumentPath(){
		Queue<Point> BFS=new LinkedList<Point>();
		Map<Point,Point> prev=new HashMap<Point,Point>();
		BFS.add(source);
		prev.put(source,null);
		boolean found=false;
		while(!BFS.isEmpty() && !found){
			Point current=BFS.poll();
			for(Point next:current.nexts)
				if(!prev.containsKey(next)){
					prev.put(next,current);
					if(next==destination){
						found=true;
						break;
					}
					BFS.add(next);
				}
		}
		if(!found)
			return null;
		List<Point> path=new LinkedList<Point>();
		for(Point point=destination;point!=null;point=prev.get(point))
			path.add(0,point);
		return path;
	}
	private void RemovePath(List<Point> path){
		for(int i=0;i+1<path.size();i++){
			path.get(i).nexts.remove(path.get(i+1));
			path.get(i+1).nexts.add(path.get(i));
		}
	}
	public void Output(){
		System.out.println(MaxFlow());
	}
	public static void main(String[] args){
		while(in.hasNext())
			new Main(in.nextInt(),in.nextInt()).Output();
	}
}