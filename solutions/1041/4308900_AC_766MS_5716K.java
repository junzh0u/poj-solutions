import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int STREET_MAX=1995;
	private LinkedHashMap<Integer,Junction> junctions=new LinkedHashMap<Integer,Junction>();
	private boolean[] travelled=new boolean[STREET_MAX+1];
	private Stack<Integer> path=new Stack<Integer>();
	private class Junction{
		private int degree=0;
		private Map<Integer,Junction> nexts=new HashMap<Integer,Junction>();
		public void AddNext(int street,Junction next){
			nexts.put(street,next);
			degree++;
			if(this==next)
				degree++;
		}
	}
	private Junction GetJunction(int ID){
		if(!junctions.containsKey(ID))
			junctions.put(ID,new Junction());
		return junctions.get(ID);
	}
	public Main(){
		Arrays.fill(travelled,false);
		while(true){
			int junctionFrom=in.nextInt();
			int junctionTo=in.nextInt();
			if(junctionFrom==0 && junctionTo==0)
				break;
			int street=in.nextInt();
			GetJunction(junctionFrom).AddNext(street,GetJunction(junctionTo));
			GetJunction(junctionTo).AddNext(street,GetJunction(junctionFrom));
		}
	}
	public void Output(){
		if(!IsEulerGraph()){
			System.out.println("Round trip does not exist.");
			return;
		}
		EulerCircuit(junctions.values().iterator().next());
		while(!path.isEmpty())
			System.out.print(path.pop()+" ");
		System.out.println();
	}
	private boolean IsEulerGraph(){
		for(Junction junction:junctions.values())
			if(junction.degree%2!=0)
				return false;
		return true;
	}
	private void EulerCircuit(Junction junction){
		for(int street:junction.nexts.keySet()){
			if(!travelled[street]){
				travelled[street]=true;
				EulerCircuit(junction.nexts.get(street));
				path.push(street);
			}
		}
	}
	public static void main(String[] args){
		while(true){
			Main trip=new Main();
			if(trip.junctions.isEmpty())
				break;
			trip.Output();
		}
	}
}