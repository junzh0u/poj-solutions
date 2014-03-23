import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private class Intersection{
		private String name=null;
		private Map<Intersection,Double> roadLength=new HashMap<Intersection,Double>();
		private Map<Intersection,Double> distances=new HashMap<Intersection,Double>();
		public Intersection(){
			distances.put(this,0.0);
		}
	}
	private class Sign{
		private class NameDistancePair implements Comparable<NameDistancePair>{
			private String name;
			private long distance;
			public NameDistancePair(String name,long distance){
				this.name=name;
				while(this.name.length()<20)
					this.name+=" ";
				this.distance=distance;
			}
			public int compareTo(NameDistancePair another){
				if(distance!=another.distance)
					return Long.signum(distance-another.distance);
				return name.compareTo(another.name);
			}
		}
		private Intersection from,to;
		private double distanceFrom;
		private Set<NameDistancePair> cityList=new TreeSet<NameDistancePair>();
		public Sign(int fromID,int toID,double distanceFrom){
			from=intersections[fromID];
			to=intersections[toID];
			this.distanceFrom=distanceFrom;
		}
		public void AddPair(String name,long distance){
			cityList.add(new NameDistancePair(name,distance));
		}
		public String toString(){
			StringBuffer buffer=new StringBuffer();
			for(NameDistancePair pair:cityList)
				buffer.append(pair.name+pair.distance+"\n");
			return buffer.toString();
		}
	}
	private Intersection[] intersections;
	private Sign[] signs;
	public Main(){
		intersections=new Intersection[in.nextInt()];
		for(int i=0;i<intersections.length;i++)
			intersections[i]=new Intersection();
		int roadNum=in.nextInt(),cityNum=in.nextInt();
		while(roadNum-->0){
			Intersection from=intersections[in.nextInt()],to=intersections[in.nextInt()];
			double distance=in.nextDouble();
			from.roadLength.put(to,distance);
			from.distances.put(to,distance);
			to.roadLength.put(from,distance);
			to.distances.put(from,distance);
		}
		while(cityNum-->0)
			intersections[in.nextInt()].name=in.next();
		signs=new Sign[in.nextInt()];
		for(int i=0;i<signs.length;i++)
			signs[i]=new Sign(in.nextInt(),in.nextInt(),in.nextDouble());
		CalculateMinDistance();
		PaintSign();
	}
	private void CalculateMinDistance(){
		for(Intersection middle:intersections)
			for(Intersection from:intersections)
				for(Intersection to:intersections)
					if(from.distances.containsKey(middle) && to.distances.containsKey(middle)){
						double distance=from.distances.get(middle)+to.distances.get(middle);
						if(!from.distances.containsKey(to) || from.distances.get(to)>distance){
							from.distances.put(to,distance);
							to.distances.put(from,distance);
						}
					}
	}
	private void PaintSign(){
		for(Sign sign:signs)
			for(Intersection city:intersections){
				if(city.name==null)
					continue;
				if(Math.abs(sign.from.distances.get(city)-sign.to.distances.get(city)-sign.from.roadLength.get(sign.to))<=1e-6)
					sign.AddPair(city.name,Math.round(sign.from.distances.get(city)-sign.distanceFrom));
			}
	}
	public void Output(){
		for(Sign sign:signs)
			System.out.println(sign);
	}
	public static void main(String[] args){
		new Main().Output();
	}
}