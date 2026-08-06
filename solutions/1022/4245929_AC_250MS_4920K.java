import java.util.*;

public class Main {
	private static final Scanner in=new Scanner(System.in);
	int[] coordinatesMax={0,0,0,0};
	int[] coordinatesMin={0,0,0,0};
	private class Cube{
		public static final int DIM=4;
		private int ID;
		private int[] gluedTo=new int[2*DIM];
		private int[] coordinates=new int[DIM];
		private boolean visited=false;
		public Cube(int ID){
			this.ID=ID;
			for(int i=0;i<2*DIM;i++)
				gluedTo[i]=in.nextInt();
		}
		public boolean IsVisited(){
			return visited;
		}
		public boolean TravelThrough(){
			for(int face=0;face<2*DIM;face++){
				if(gluedTo[face]==0)
					continue;
				if(gluedTo[face]==ID || !cubes.containsKey(gluedTo[face]))
					return false;
				Cube next=cubes.get(gluedTo[face]);
				if(next.gluedTo[(face%2==0)?(face+1):(face-1)]!=ID)
					return false;
				if(next.IsVisited())
					continue;
				int[] newCoordinates=new int[DIM];
				System.arraycopy(coordinates,0,newCoordinates,0,DIM);
				newCoordinates[face/2]+=(face%2==0)?1:-1;
				next.SetCoordinate(newCoordinates);
				if(!next.TravelThrough())
					return false;
			}
			return true;
		}
		public void SetCoordinate(int[] newCoordinates){
			System.arraycopy(newCoordinates,0,coordinates,0,DIM);
			for(int dim=0;dim<DIM;dim++){
				if(coordinatesMax[dim]<coordinates[dim])
					coordinatesMax[dim]=coordinates[dim];
				if(coordinatesMin[dim]>coordinates[dim])
					coordinatesMin[dim]=coordinates[dim];
			}
			visited=true;
		}
		public int GetCoordinate(int dim){
			return coordinates[dim];
		}
	}
	Map<Integer,Cube> cubes=new HashMap<Integer,Cube>();
	boolean consistent=false;
	public Main(){
		int cubeNum=in.nextInt();
		while(cubeNum-->0){
			int ID=in.nextInt();
			Cube cube=new Cube(ID);
			cubes.put(ID,cube);
		}
		Set<Integer> IDs=cubes.keySet();
		Iterator<Integer> iter=IDs.iterator();
		Cube begin=cubes.get(iter.next());
		begin.SetCoordinate(new int[]{0,0,0,0});
		if(begin.TravelThrough()){
			consistent=true;
			for(iter=IDs.iterator();iter.hasNext();)
				if(!cubes.get(iter.next()).IsVisited()){
					consistent=false;
					break;
				}
		}
	}
	public void Output(){
		if(!consistent){
			System.out.println("Inconsistent");
			return;
		}
		int volume=1;
		for(int dim=0;dim<Cube.DIM;dim++)
			volume*=coordinatesMax[dim]-coordinatesMin[dim]+1;
		System.out.println(volume);
	}
	public static void main(String[] args) {
		int caseNum=in.nextInt();
		while(caseNum-->0){
			Main pack=new Main();
			pack.Output();
		}
	}
}