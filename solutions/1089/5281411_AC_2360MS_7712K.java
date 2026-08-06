import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private class Interval implements Comparable<Interval>{
		private int lower,higher;
		public Interval(int lower,int higher){
			this.lower=lower;
			this.higher=higher;
		}
		public int compareTo(Interval another){
			if(lower!=another.lower)
				return Integer.signum(lower-another.lower);
			return Integer.signum(another.higher-higher);
		}
		public String toString(){
			return lower+" "+higher;
		}
	}
	private List<Interval> intervals=new ArrayList<Interval>();
	public Main(){
		int intervalNum=in.nextInt();
		while(intervalNum-->0)
			intervals.add(new Interval(in.nextInt(),in.nextInt()));
		Collections.sort(intervals);
		/*for(int i=0;i<intervals.size();i++)
			while(i+1<intervals.size() && intervals.get(i+1).lower<=intervals.get(i).higher){
				if(intervals.get(i).higher<intervals.get(i+1).higher)
					intervals.get(i).higher=intervals.get(i+1).higher;
				intervals.remove(i+1);
			}*/
	}
	public void Output(){
		for(int i=0;i<intervals.size();){
			int next=i+1;
			while(next<intervals.size()){
				if(intervals.get(i).higher<intervals.get(next).lower)
					break;
				if(intervals.get(i).higher<intervals.get(next).higher)
					intervals.get(i).higher=intervals.get(next).higher;
				next++;
			}
			System.out.println(intervals.get(i));
			i=next;
		}
	}
	public static void main(String[] args){
		new Main().Output();
	}
}